import numpy as np
import random
import chainer
import csv

import const
import network

def abst_field_neural():
    csv_file = open(const.csv_path, 'r')

    csv_data = csv.reader(csv_file, delimiter=',', skipinitialspace=True)

    data = []
    for raw in csv_data:
        data.append(list(map(float,raw)))

    random.shuffle(data)

    siz = min(const.siz, len(data))

    inp_data = np.zeros((siz, 29), dtype=np.float32)
    out_data = np.zeros(siz, dtype=np.int32)

    # 正規化
    f = lambda ma,mi,va: (va-mi)/(ma-mi)

    ma_list = [1.5, 15, -1.5, 5, 1.5, 16.0, 16.0, 12.0, 12.0]
    mi_list = [0.0, 2, -10, -2, -1.5, -16.0, -16.0, 8.0, 8.0]

    for index in range(siz):
        for i in range(29):
            if i < 9:
                inp_data[index][i] = f(ma_list[i], mi_list[i], data[index][i+1])
            elif i == 18 or i == 28:
                inp_data[index][i] = data[index][i+1] / 3.0
            else:
                inp_data[index][i] = 0.5 + data[index][i+1] / 20.0

        out_data[index] = 0 if data[index][30] == data[index][31] else 1 if data[index][30] > data[index][31] else 2
    
    train_size = int((1.0 - const.test_data_per) * siz)

    train_data = chainer.datasets.TupleDataset(inp_data[:train_size], out_data[:train_size])
    test_data = chainer.datasets.TupleDataset(inp_data[train_size:], out_data[train_size:])

    network.calc_neural([100, 100, 3], train_data, test_data, const.result_path + '_abst_field/', '_abst_field', int(min(const.max_train, train_size)), int(min(const.max_test, int(const.test_data_per * len(data)))), const.epoch)


abst_field_neural()