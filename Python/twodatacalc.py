import numpy as np
import random
import chainer

import const
import readcsv
import network


def two_data_calc(inp1, inp2, csv_data, hid_unit):
    random.shuffle(csv_data)

    train_size = int((1.0 - const.test_data_per) * len(csv_data))

    inp_data = np.empty((len(csv_data), 2), dtype=np.float32)
    out_data = np.empty((len(csv_data), 1), dtype=np.float32)

    for index in range(len(csv_data)):
        inp_data[index][0] = csv_data[index][inp1]
        inp_data[index][1] = csv_data[index][inp2]
        out_data[index][0] = csv_data[index][-1]
    
    train_data = chainer.datasets.TupleDataset(inp_data[:train_size], out_data[:train_size])
    test_data = chainer.datasets.TupleDataset(inp_data[train_size:], out_data[train_size:])

    data_id = '_' + str(inp1) + '_' + str(inp2)
    data_path = const.result_path + data_id + '/'

    network.calc_neural([hid_unit, 1], train_data, test_data, data_path, data_id, 10000, 100, 50)

def func_two_data():

    csv_data = readcsv.read_csv(const.csv_path)
    data_size = len(csv_data[0]) - 1
    data_size = 2

    for count_1 in range(data_size):
        for count_2 in range(count_1 + 1):
            two_data_calc(count_1, count_2, csv_data, const.epoch)
