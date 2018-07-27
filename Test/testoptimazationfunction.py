import numpy as np
import csv
import copy
import random
import struct
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

common_path = '../Data/TestOptimazationFunction/'

csv_path = common_path + 'input.csv'
result_path = common_path + 'result'
save_model_path = 'and'
save_png_path = 'image'
loss_file_path = 'loss'


# テスト用データの占める割合
test_data_per = 0.2

train_batch_size = 10000
test_batch_size = 100

# 学習の試行回数(これをN^2/2回続ける)
epoch = 50

hid_unit = 50

class NetWork(chainer.Chain):

    # 層のユニット数に
    def __init__(self, n_layers):
        # 継承してる
        super(NetWork, self).__init__()
        with self.init_scope():
            self.l1 = L.Linear(None, n_layers[0])
            self.l2 = L.Linear(None, n_layers[1])
    
    # 順計算
    def __call__(self, x):
        return self.l2(F.relu(self.l1(x)))

def read_csv():
    csv_file = open(csv_path, 'r')

    data = csv.reader(csv_file, delimiter=',', skipinitialspace=True)

    ret_data = []
    field_data = []

    for row in data:
        if row[0] == '-1':
            field_data = row[1:]
        else:
            ret_data.append(list(map(float,field_data + row)))
            # 勝敗予測より得点差予測の方がよさそうなので
            ret_data[-1][-1] = 1.0 if (int(ret_data[-1][-1]) > 0) else 0.0

    csv_file.close()

    return ret_data


csv_data = read_csv()

def two_data_calc(inp1, inp2):
    random.shuffle(csv_data)

    train_size = int((1.0 - test_data_per) * len(csv_data))

    inp_data = np.empty((len(csv_data), 2), dtype=np.float32)
    out_data = np.empty((len(csv_data), 1), dtype=np.float32)

    for index in range(len(csv_data)):
        inp_data[index][0] = csv_data[index][inp1]
        inp_data[index][1] = csv_data[index][inp2]
        out_data[index][0] = csv_data[index][-1]
    
    train_data = chainer.datasets.TupleDataset(inp_data[:train_size], out_data[:train_size])
    test_data = chainer.datasets.TupleDataset(inp_data[train_size:], out_data[train_size:])

    data_id = '_' + str(inp1) + '_' + str(inp2)
    data_path = result_path + data_id + '/'

    # 相対パスと接尾辞
    '''
    print(data_path)
    print(data_id)
    '''
    calc_neural([hid_unit, 1], train_data, test_data, data_path, data_id)
    
# 隠れ層と出力層の数を配列で指定してtrainとtestのデータをTupleDataSetで作って、保存先フォルダの(相対)パスとファイル名のsuffixを指定する
def calc_neural(layers, train, test, data_path, data_suffix):

    net = NetWork(layers)

    accfun = lambda x, t: F.sum(1 - abs(x-t))/x.size 
    model = L.Classifier(net, lossfun=F.mean_squared_error, accfun=accfun)
    optimizer = chainer.optimizers.Adam()
    optimizer.setup(model)

    train_iter = chainer.iterators.SerialIterator(train, train_batch_size)
    test_iter = chainer.iterators.SerialIterator(test, test_batch_size, repeat=False)

    updater = training.StandardUpdater(train_iter, optimizer, device=-1)

    trainer = training.Trainer(updater, (epoch, 'epoch'), out=data_path)

    trainer.extend(extensions.Evaluator(test_iter, model, device=-1))
    trainer.extend(extensions.dump_graph('main/loss'))
    trainer.extend(extensions.LogReport(trigger=(1, 'epoch')))
    trainer.extend(extensions.PlotReport(
        ['main/loss', 'validation/main/loss'], x_key='epoch',
        file_name= loss_file_path + data_suffix + '.png'
    ))
    trainer.extend(extensions.PrintReport(
        ['epoch', 'main/loss', 'main/accuracy', 'validation/main/loss', 'validation/main/accuracy'],
    ))

    trainer.run()

    chainer.serializers.save_npz(data_path + save_model_path + data_suffix + '.model', model)

    # ここの出力は入力が2次元な場合のみなので
    '''
    # net((n,2)のarray)で予測した値が出せるはず
    # ここにx,y各1/graph_div刻みでグラフを描画する

    graph_div = 100

    inparr_0 = np.linspace(0.0, 1.0, graph_div + 1, dtype=np.float32)
    inparr_1 = np.linspace(0.0, 1.0, graph_div + 1, dtype=np.float32)
    inparr_2, inparr_3 = np.meshgrid(inparr_0, inparr_1)
    outarr = np.zeros(((graph_div + 1, graph_div + 1)), dtype=np.float32)

    for i0 in range(graph_div + 1):
        for i1 in range(graph_div + 1):
            outarr[i0, i1] = net(np.array([[inparr_0[i0], inparr_1[i1]]], dtype=np.float32))[0][0].data
    
    plt.figure(figsize=(5, 3.5))
    ax = plt.subplot(1, 1, 1, projection='3d')
    ax.plot_surface(inparr_2, inparr_3, outarr)
    ax.view_init(60, 40)
    plt.savefig(data_path + save_png_path + data_suffix + '.png')
    '''


def main():

    # field_data.size + ret_data.size - 1になる(最後尾には勝率が来るため)
    data_size = len(csv_data[0]) - 1

    for count_1 in range(data_size):
        for count_2 in range(count_1 + 1):
            two_data_calc(count_1, count_2)



if __name__ == '__main__':
    main()