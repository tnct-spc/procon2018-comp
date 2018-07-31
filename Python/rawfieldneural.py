import numpy as np
import random
import chainer

import const
import readbinary
import network

bin_data = readbinary.read_binary()

def raw_field_neural():
    random.shuffle(bin_data)

    inp_data = np.zeros((len(bin_data), 304), dtype=np.float32)
    out_data = np.empty((len(bin_data), 1), dtype=np.float32)

    train_size = int((1.0 - const.test_data_per) * len(bin_data))

    # sizeで2 pointで12*12=144 stateで12*12=144 turnで2 agentで2*2*2=8 point2*2=4 sum:304

    for index in range(len(bin_data)):

        for i in range(2):
            inp_data[index][i] = bin_data[index]['siz'][i]

        for i in range(bin_data[index]['siz'][0]):
            for j in range(bin_data[index]['siz'][1]):
                inp_data[index][i * 12 + j + 2] = bin_data[index]['state'][i][j]
                inp_data[index][i * 12 + j + 146] = bin_data[index]['value'][i][j]

        inp_data[index][290] = bin_data[index]['now_turn']
        inp_data[index][291] = bin_data[index]['final_turn']
        
        for i in range(2):
            for j in range(2):
                for k in range(2):
                    inp_data[index][292 + i * 4 + j * 2 + k] = bin_data[index]['agent'][i][j][k]

        for i in range(2):
            for j in range(2):
                inp_data[index][300 + i * 2 + j] = bin_data[index]['point'][i][j]

        out_data[index][0] = bin_data[index]['diff']

    train_data = chainer.datasets.TupleDataset(inp_data[:train_size], out_data[:train_size])
    test_data = chainer.datasets.TupleDataset(inp_data[train_size:], out_data[train_size:])

    network.calc_neural([800, 1200, 800, 1], train_data, test_data, const.result_path + '_raw_field/', '_raw_field', int(min(1000, train_size * 0.1)), int(min(200, train_size * 0.05)), 100000)


raw_field_neural()