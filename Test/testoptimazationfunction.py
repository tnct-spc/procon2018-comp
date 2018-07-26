import numpy as np
import csv
import random
import struct
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

common_path = '../Data/TestOptimazationFunction/'

csv_path = common_path + 'input.csv'
result_path = common_path + 'result'
save_model_path = common_path + 'and'
save_dat_path = common_path + 'and'
save_png_path = common_path + 'image'
loss_file_path = common_path + 'loss'

# field_data.size + ret_data.size - 1になる(最後尾には勝率が来るため)
data_size = 10

data_row = None
train_batch_size = 100
test_batch_size = 100

# 学習の試行回数(これをN^2/2回続ける)
epoch = 100

csv_data = None

hid_unit = 5

class NetWork(chainer.Chain):

    def __init__(self, n_inp, n_hid, n_out):
        # 継承してる
        super(NetWork, self).__init__(
            l1 = L.Linear(n_inp, n_hid),
            l2 = L.Linear(n_hid, n_out)
        )
    
    # 順計算
    def __call__(self, inp):
        hidden = F.sigmoid(self.l1(inp))
        out = self.l2(hidden)
        return out

def read_csv():
    csv_file = open(csv_path, 'r')

    data = csv.reader(csv_file, delimiter='r', skipinitalshape=True)

    ret_data = []
    field_data = []

    for row in data:
        if row[0] == '0':
            field_data = row[1:]
        else:
            ret_data.append(field_data + row)

    csv_file.close()

    return ret_data


csv_data = read_csv()
data_row = len(csv_data)

def make_data(inp1, inp2):

    random.shuffle(csv_data)

    inp_data = []
    out_data = []

    for index in range(train_batch_size + test_batch_size):
        inp_data.append(csv_data[index][inp1], csv_data[index][inp2])
        out_data.append(csv_data[index][-1])
    
    train_data = chainer.datasets.TupleDataset(inp_data[:train_batch_size], out_data[:train_batch_size])
    test_data = chainer.datasets.TupleDataset(inp_data[-test_batch_size:], inp_data[-test_batch_size:])

    return train_data, test_data


def calc(inp1, inp2):
    train,test = make_data(inp1, inp2)

    net = NetWork(2, hid_unit, 1)

    model = L.Classifier(net, lossfun=F.mean_squared_error)
    optimizer = chainer.optimizers.Adam()
    optimizer.setup(model)

    train_iter = chainer.iterators.SerialIterator(train, train_batch_size)
    test_iter = chainer.iterators.SerialIterator(test, test_batch_size, repeat=False, shuffle=False)

    updater = training.StandardUpdater(train_iter, optimizer, device=-1)

    trainer = training.Trainer(updater, (epoch, 'epoch'), out=result_path)

    trainer.extend(extensions.Evaluator(test_iter, model, device=-1))
    trainer.extend(extensions.dump_graph('loss'))
    trainer.extend(extensions.LogReport(trigger=(1, 'epoch')))
    trainer.extend(extensions.PlotReport(
        ['main/loss', 'validation/main/loss'], x_key='epoch',
        file_name=loss_file_path + '_' + str(inp1) + '_' + str(inp2) + '.png'
    ))
    trainer.extend(extensions.PrintReport(
        ['epoch', 'main/loss', 'main/accuracy', 'validation/main/loss', 'validation/main/accuracy'],
    ))

    trainer.run()
    chainer.serializers.save_npz(save_model_path + '_' +  str(inp1) + '_' + str(inp2) + '.model', model)

    # ここから重みの整形とbatの出力処理まで行う
    b_arr = bytearray()

    for v in model.predictor.l1.W.data.reshape(2 * hid_unit):
        b_arr += struct.pack('f', v)
    for v in model.predictor.l1.b.data:
        b_arr += struct.pack('f', v)

    for v in model.predictor.l2.W.data.reshape(hid_unit * 1):
        b_arr += struct.pack('f', v)
    for v in model.predictor.l2.b.data:
        b_arr += struct.pack('f', v)

    open(save_dat_path + '_' + str(inp1) + '_' + str(inp2) + '.dat', 'w').write(b_arr)

    # net((n,2)のarray)で予測した値が出せるはず


def main():

    # 2つの入力と1つの出力を持つモデル
    # ここにtrainとtestを宣言する

    for row in csv_data:
        print(row)

    for count_1 in range(data_size):
        for count_2 in range(count_1 + 1):
            calc(count_1, count_2)



if __name__ == '__main__':
    main()