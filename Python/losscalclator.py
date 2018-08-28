import numpy as np

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import const
import network
import wrap

class LossCalclator():

    def __init__(self, n_layers, pathes):
        self.wrappers = [Wrap(n_layers, path) for path in pathes]

    # これは29層のパラメータが与えられる
    def paramcalc(self, x):
        point = 0.0
        ret = predict(x)
        for wrap in self.wrappers:
            point += 0 if ret == 1 else 0.5 if ret == 0 else 1
        return point
