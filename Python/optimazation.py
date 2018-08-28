import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import losscalclator
import const


class LossCalcNet(chainer.Chain):

    def __init__(self, n_layers, pathes):
        super(LossCalcNet, self).__init__()
        with self.init_scope():
            self.l1 = L.Linear(None, n_layers[0])
            self.l2 = L.Linear(None, n_layers[1])
            self.l3 = L.Linear(None, n_layers[2])
            self.losscalc = losscalclator.LossCalclator(n_layers[:2] + [3], pathes)
    
    def __call__(self, x):
        print('__call__ : {}'.format(x.shape))
        # ここで最適なパラメータを出している
        h = F.sigmoid(self.l3(F.dropout(F.relu(self.l2(F.dropout(F.relu(self.l1(x))))))))

        y = np.zeros((x.shape[0], 1), dtype=np.float32)
        for i in range(x.shape[0]):
            y[i][0] = self.losscalc.losscalc(np.hstack((x[i],h[i].data * 10)))
        return y

def func(x,t):
    lfunc = lambda x, t: F.sum(1 - x)/x.size 
    y = lfunc(x,t)
    print('func : {}'.format(y.data))
    return y

def accfun(x,t):
    laccfun = lambda x, t: F.sum(x)/x.size 
    y = laccfun(x,t)
    print('accfun : {}'.format(y.data))
    return y

# 10層が与えられた時の9層の最適化
def optimazation_param(layers, train, test, data_path, data_suffix, train_batch_size, test_batch_size, epoch, pathes):

    net = LossCalcNet(layers, pathes)

    model = L.Classifier(net, lossfun=func, accfun=accfun)

    optimizer = chainer.optimizers.Adam()
    optimizer.setup(model)

    if const.use_weight_data:
        chainer.serializers.load_npz(const.saved_path + const.load_model_name + data_suffix + '.npz', model)

    train_iter = chainer.iterators.SerialIterator(train, train_batch_size)
    test_iter = chainer.iterators.SerialIterator(test, test_batch_size, repeat=False)

    updater = training.StandardUpdater(train_iter, optimizer)

    trainer = training.Trainer(updater, (epoch, 'epoch'), out=data_path)

    trainer.extend(extensions.Evaluator(test_iter, model))
    trainer.extend(extensions.dump_graph('main/loss'))
    trainer.extend(extensions.LogReport(trigger=(1, 'epoch')))
    trainer.extend(extensions.PlotReport(
        ['main/loss', 'validation/main/loss'], x_key='epoch',
        file_name= const.loss_file_name + data_suffix + '.png'
    ))
    trainer.extend(extensions.PrintReport(
        ['epoch', 'main/loss', 'main/accuracy', 'validation/main/loss', 'validation/main/accuracy'],
    ))

    trainer.run()

def main():
    ma_list = [1.5, 15, -1.5, 5, 1.5, 16.0, 16.0, 12.0, 12.0]
    mi_list = [0.0, 2, -10, -2, -1.5, -16.0, -16.0, 8.0, 8.0]
    inp_data = np.zeros((const.siz, 9), dtype=np.float32)
    out_data = np.zeros((const.siz, 1), dtype=np.float32)

    for index in range(const.siz):
        out_data[index][0] = 1.0;

        for i in range(9):
            inp_data[index][i] = np.random.rand() * (ma_list[i] - mi_list[i]) + mi_list[i]

    train_size = int((1.0 - const.test_data_per) * const.siz)

    train_data = chainer.datasets.TupleDataset(inp_data[:train_size], out_data[:train_size])
    test_data = chainer.datasets.TupleDataset(inp_data[train_size:], out_data[train_size:])

    optimazation_param([100, 100, 10], train_data, test_data, const.result_path + '_optimazation/', '_optimazation', int(min(const.max_train, train_size)), int(min(const.max_test, int(const.test_data_per * const.siz))), const.epoch,
            ['../Data/TestAlgorithmPlayout/results/result_' + str(i + 1) + '/and_abst_field.npz' for i in range(6)])

if __name__ == '__main__':
    main()
