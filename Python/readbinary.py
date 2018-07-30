import const

import psutil

def unpack_field(inp_side, f, field_list):


    field = {}

    siz = int.from_bytes(f.read(1), 'little')

    if siz == 0:
        return False
    
    field['siz'] = [(siz >> 4) & 15, siz & 15]

    field['state'] = [[0 for i in range(field['siz'][1])] for j in range(field['siz'][0])]
    field['value'] = [[0 for i in range(field['siz'][1])] for j in range(field['siz'][0])]

    for x in range(field['siz'][0]):
        for y in range(field['siz'][1]):

            dat = int.from_bytes(f.read(1), 'little')

            field['state'][x][y] = dat >> 6
            field['value'][x][y] = dat & ((1 << 6) - 1) - 16
    
    field['now_turn'] = int.from_bytes(f.read(1), 'little')
    field['final_turn'] = int.from_bytes(f.read(1), 'little')

    field['agent'] = [[[0 for i in range(2)] for j in range(2)] for k in range(2)]

    for side in range(2):
        for agent in range(2):
            dat = int.from_bytes(f.read(1), 'little')

            field['agent'][side][agent][0] = dat >> 4
            field['agent'][side][agent][1] = dat & ((1 << 4) - 1)
    
    field['move'] = [[0 for i in range(3)] for j in range(2)]

    for agent in range(2):
        dat = int.from_bytes(f.read(1), 'little')
        for index in range(3):
            field['move'][agent][index] = (dat >> (4 - index * 2) & 3) - (1 if index else 0)
    
    field['diff'] = int.from_bytes(f.read(2), 'big') - (1 << 15)

    field['side'] = inp_side
    
    field_list.append(field)

    return psutil.virtual_memory().percent < 70.0

def read_binary():

    file = open(const.binary_path, 'rb')

    move_list = []

    side = 0
    while unpack_field(side, file, move_list):
        side ^= 1
    
    return move_list



read_binary()