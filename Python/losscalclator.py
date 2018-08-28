import numpy as np

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import const
import network
import wrap

playout_count = 1000
rand_bound = 0.5

class LossCalclator():

    def __init__(self, n_layers, pathes):
        self.wrappers = [wrap.Wrap(n_layers, path) for path in pathes]

    # これは29層のパラメータが与えられる
    def paramcalc(self, x):
        point = 0.0
        for wrap in self.wrappers:
            ret = wrap.predict(x)
            point += 0 if ret == 1 else 0.5 if ret == 0 else 1
        return point

    # 19層のパラメータが与えられて、そのパラメータでの勝ちやすさを返す
    def losscalc(self, x):

        value = 0.0
        max_value = 1.0 * len(self.wrappers) * playout_count

        for count in range(playout_count):
            arr = np.zeros(10, dtype=np.float32)
            for i in range(10):
                arr[i] = np.random.rand() * 20.0 - 10
                # arr[i] = (np.random.rand() * 20.0 - 10 if np.random.rand() > rand_bound else max(min(x[9+i] + np.random.rand() * 8 - 4, 10.0), -10.0))
            y = np.hstack((x, arr))
            value += self.paramcalc(y)

        return value / max_value
