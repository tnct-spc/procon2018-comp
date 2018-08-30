import numpy as np
import random

import losscalclator
import const

calcnum = 10000


def main():
    pathes = ['../Data/TestAlgorithmPlayout/results/result_' + str(i + 1) + '/and_abst_field.npz' for i in range(6)]

    targets = [[1.6, 1.2, -0.7, 1.2, 1.2, -0.3 ,1.2, -0.4, -0.4, 1.5]]
    net = losscalclator.LossCalclator([100, 100, 3], pathes)

    for count in range(calcnum):
        target = np.array(random.choice(targets), dtype=np.float32)
        params = (np.random.rand(10) * 20 - 10).astype(np.float32)
        
        par = net.parcalc(np.hstack((params, target)))
        if par > 0.9:
            print('count : {}'.format(count + 1))
            print('par : {}'.format(par))
            print('diagonal_move = {}'.format(params[9]), end=';\n')
            print('const_values = {', end='')
            for val in params[:8]:
                print(val, end=', ')
            print(params[9], end='')
            print('}', end=';\n\n')


if __name__ == '__main__':
    main()
