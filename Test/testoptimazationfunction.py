import numpy as np
import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

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

def main():

    # 2つの入力と1つの出力を持つモデル
    # ここにtrainとtestを宣言する

    '''
    train = make_data(10000)
    test = make_data(1000)
    '''

    test_batch_size = 100
    train_batch_size = 100
    epoch = 100

    model = L.Classifier(NetWork(2, 3, 1), lossfun=F.mean_squared_error)
    optimizer = chainer.optimizers.Adam()
    optimizer.setup(model)

    train_iter = chainer.iterators.SerialIterator(train, train_batch_size)
    test_iter = chainer.iterators.SerialIterator(test, test_batch_size, repeat=False, shuffle=False)
    # test_iter = chainer.iterators.SerialIterator(test, test_batch_size)

    updater = training.StandardUpdater(train_iter, optimizer, device=-1)

    trainer = training.Trainer(updater, (epoch, 'epoch'), out='result')

    trainer.extend(extensions.Evaluator(test_iter, model, device=-1))
    '''
    trainer.extend(extensions.dump_graph('loss'))
    trainer.extend(extensions.LogReport(trigger=(1, 'epoch')))
    trainer.extend(extensions.PrintReport(
        ['epoch', 'main/loss', 'main/accuracy', 'validation/main/loss', 'validation/main/accuracy'],
    ))
    '''

    trainer.run()
    chainer.serializers.save_npz('and.model',model)


if __name__ == '__main__':
    main()