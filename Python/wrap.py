import numpy as np

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import const
import network

class Wrap():

    def __init__(self, n_layers, path):
        self.net = network.NetWork(n_layers)
        self.model = L.Classifier(self.net)
        chainer.serializers.load_npz(path, self.model)

    def predict(self, x):
        x = np.reshape(x, (1, x.shape[0]))
        h = self.net.predict_(x)
        print('predict : {}'.format(h))
        return h
