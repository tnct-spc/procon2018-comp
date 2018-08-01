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
            if field['state'][x][y] and inp_side:
                field['state'][x][y] = 2 if field['state'][x][y] == 1 else 1

            field['value'][x][y] = dat & ((1 << 6) - 1) - 16
    
    field['now_turn'] = int.from_bytes(f.read(1), 'little')
    field['final_turn'] = int.from_bytes(f.read(1), 'little')

    field['agent'] = [[[0 for i in range(2)] for j in range(2)] for k in range(2)]

    for side in range(2):
        for agent in range(2):
            dat = int.from_bytes(f.read(1), 'little')

            field['agent'][side ^ inp_side][agent][0] = dat >> 4
            field['agent'][side ^ inp_side][agent][1] = dat & ((1 << 4) - 1)
    

    field['point'] = [[0 for i in range(2)] for j in range(2)]


    for side in range(2):
        point = int.from_bytes(f.read(3), 'big')

        for index in range(2):
            field['point'][side ^ inp_side][index ^ 1] = (point >> (12 * index)) & ((1 << 12) - 1)


    field['move'] = [[0 for i in range(3)] for j in range(2)]

    for agent in range(2):
        dat = int.from_bytes(f.read(1), 'little')
        for index in range(3):
            field['move'][agent][index] = (dat >> (4 - index * 2) & 3) - (1 if index else 0)
    

    field['diff'] = int.from_bytes(f.read(2), 'big') - (1 << 15)

    field['side'] = inp_side

    
    field_list.append(field)

    return psutil.virtual_memory().percent < 85.0

def read_binary():

    file = open(const.binary_path, 'rb')

    move_list = []

    count = 0
    while unpack_field(count & 1, file, move_list) and count < 5:
        count += 1

    print('binary_read finished count: {}'.format(count))

    for field in move_list:
        print(field)
    
    return move_list



read_binary()