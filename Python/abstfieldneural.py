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
        data.append(raw)

    random.shuffle(data)

    inp_data = np.zeros((len(data), 30), dtype=np.float32)
    out_data = np.empty((len(data), 3), dtype=np.float32)

    for index in range(min(100000,len(data))):
        for i in range(30):
            inp_data[index][i] = data[index][i]
        out_data[index][0 if data[index][30] == data[index][31] else 1 if data[index][30] > data[index][31] else 2] = 1.0

    train_size = int((1.0 - const.test_data_per) * len(data))

    train_data = chainer.datasets.TupleDataset(inp_data[:train_size], out_data[:train_size])
    test_data = chainer.datasets.TupleDataset(inp_data[train_size:], out_data[train_size:])

    network.calc_neural([30, 30, 3], train_data, test_data, const.result_path + '_abst_field/', '_abst_field', int(min(const.max_train, train_size)), int(min(const.max_test, int(const.test_data_per * len(data)))), const.epoch)


abst_field_neural()