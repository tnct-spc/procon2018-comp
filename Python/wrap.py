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
        return self.net.predict_(x)
