import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import const


class NetWork(chainer.Chain):

    # ????????
    def __init__(self, n_layers):
        # ?????
        super(NetWork, self).__init__()
        with self.init_scope():
            self.l1 = L.Linear(None, n_layers[0])
            self.l2 = L.Linear(None, n_layers[1])
    
    # ???
    def __call__(self, x):
        return self.l2(F.dropout(F.relu(self.l1(x))))



# 隠れ層と出力層の数を配列で指定してtrainとtestのデータをTupleDataSetで作って、保存先フォルダの(相対)パスとファイル名のsuffixを指定する
def calc_neural(layers, train, test, data_path, data_suffix, train_batch_size, test_batch_size, epoch):

    net = NetWork(layers)

    accfun = lambda x, t: F.sum(1 - abs(x-t))/x.size 
    model = L.Classifier(net, lossfun=F.mean_squared_error, accfun=accfun)
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

    chainer.serializers.save_npz(data_path + const.save_model_name + data_suffix + '.npz', model)
