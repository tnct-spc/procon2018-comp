import numpy as np

"""
import os
import sys
sys.path.append(os.pardir)
"""

import communication

# g++ -DPIC -shared -fPIC -o basic.so communication.cpp -lboost_python -lpython3.6m -I/usr/include/python3.6m -L../Field -I../Field

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import chainer
from chainer import links as L
from chainer import functions as F
from chainer import training
from chainer.training import extensions

import chainerrl

# 盤面の状態数
# siz:2 st_va:12*12*2=288 tu:2 ag:2*2*2=8 po:2*2=4 sum:304
field_val = 304
# 行動のパターン数
# 9*2=18 全ての行動に重みを取って最大値を取る(?)
n_move = 324

n_playout = 20000
debug_time = 100
save_time = 10000

gsid = False

# fi:numpy[304] done
class Field():
    def __init__(self):
        self.com = communication.Communication()
        self.fi = np.array(self.com.resetField(), dtype=np.float32)
        self.done = False

    def reset(self):
        self.fi = np.array(self.com.resetField(), dtype=np.float32)
        self.done = False
        # fieldのランダム生成 c++に投げる
        pass

    def move(self, act):
        # act = [, [[points], [points]], [[points], [points]], ]
        moves = []
        for sid in range(2):
            ac = act[sid]
            # [0,324)を[0,18),[0,18)にしている
            lis = [(ac // (81 * 2)) * 9 + (ac // 9) % 9, ((ac // 81) % 2) * 9 + ac % 9]
            for x in lis:
                moves.append(int(x))

        self.fi = np.array(self.com.move(moves), dtype=np.float32)
        # 終わったかどうか
        self.done = (self.fi[290] == self.fi[291])
        # canPutを実行 falseなら強制lose それ以外ならmove
        # act[2] 両方[0,324)

    def winner(self):
        return self.com.winner()
        # 勝者を返す

class RandAct:
    def __init__(self, f):
        self.f = f #field
        self.com = communication.Communication()
        self.random_count = 0

    def random_action_func(self):
        # C++を呼んで、有効手を1つ返す)
        lis = self.com.random(int(gsid))
        val = 81 * 2 * (lis[0] == 2) + lis[1] * 9 + 81 * (lis[2] == 2) + lis[3]
        return val

class QFunction(chainer.Chain):

    def __init__(self, n_layers):
        super(QFunction, self).__init__()
        with self.init_scope():
            self.l1 = L.Linear(None, n_layers[0])
            self.l2 = L.Linear(None, n_layers[1])
            self.l3 = L.Linear(None, n_move)

    def __call__(self, x):
        # 9種類の動きとis_deleteがあるので10種類 分類問題
        h = F.relu(self.l1(x))
        h = F.relu(self.l2(h))
        h = self.l3(h)
        h = chainerrl.action_value.DiscreteActionValue(h)
        return h

def revside(arr):
    for i in range(144):
        if arr[i + 2]:
            arr[i + 2] = 2 if arr[i + 2] == 1 else 1

    for i in range(4):
        arr[292 + i], arr[296 + i] = arr[296 + i], arr[292 + i]

    for i in range(2):
        arr[300 + i], arr[302 + i] = arr[302 + i], arr[300 + i]

f = Field()

ra = RandAct(f)

# ここ適当にやる
q_func = QFunction([50, 50])


optimizer = chainer.optimizers.Adam()
optimizer.setup(q_func)


# 割引率らしい
gamma = 0.95

# chainerrl.explorers.ConstantEpsilonGreedy でもよい linearは線形って事で
# explorer = chainerrl.explorers.LinearDecayEpsilonGreedy(start_epsilon=1.0, end_epsilon=0.3, decay_steps=50000, random_action_func=ra.random_action_func)
explorer = chainerrl.explorers.LinearDecayEpsilonGreedy(start_epsilon=0.5, end_epsilon=0.3, decay_steps=50000, random_action_func=ra.random_action_func)

replay_buffer = chainerrl.replay_buffer.ReplayBuffer(capacity= 10 ** 6)

agent_p1 = chainerrl.agents.DoubleDQN(q_func, optimizer, replay_buffer, gamma, explorer, replay_start_size=500)
agent_p2 = chainerrl.agents.DoubleDQN(q_func, optimizer, replay_buffer, gamma, explorer, replay_start_size=500)

result = [0 for i in range(3)]

for i in range(n_playout):
    f.reset()
    agents = [agent_p1, agent_p2]

    reward = 0

    while not f.done:

        action = []
        for sid in range(2):
            act = []
            # rewardは必ず0になる気がするけど…
            gsid = sid

            act_res = agents[sid].act_and_train(f.fi.copy(), reward)
            action.append(act_res)

            revside(f.fi)

        # ここでターンの終了処理
        f.move(action)

    win = f.winner()
    result[win + 1] += 1

    # print(f.fi)
    # print('win : {}'.format(win))
    for sid in range(2):
        if agents[sid].last_state is not None:
            agents[sid].stop_episode_and_train(f.fi, win * (-1 if sid else 1), True)
        revside(f.fi)

    if not ((i + 1) % debug_time):
        print('episode:{}, rnd:{}, miss:{}, draw:{}, win:{}, statistics:{}, epsilon:{}'.format(i, ra.random_count, result[0], result[1], result[2], agent_p1.get_statistics(), agent_p2.explorer.epsilon))
        result = [0 for i in range(3)]
        ra.random_count = 0
    if not ((i + 1) % save_time):
        agent_p1.save('result_' + str(i))

print('finished')
