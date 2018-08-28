import numpy as np

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import const
import network

class wrap():

    def __init__(self, n_layers):
        self.net = Network(n_layers)

    def predict(self, x):
        return self.net.predict_(x)
