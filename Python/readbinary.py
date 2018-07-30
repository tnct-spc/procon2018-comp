import const

def unpack_field(f, field_list):

    field = {}

    siz = f.read(1)

    if siz == '':
        return False
    
    field['siz'] = [siz & 7, (siz >> 4) & 7]

    field['state'] = [[0] * siz[1]] * siz[0]
    field['value'] = [[0] * siz[1]] * siz[0]

    for x in range(field['siz'][0]):
        for y in range(field['siz'][1]):
            dat = f.read(1)
            field['state'][x][y] = dat >> 6
            field['value'][x][y] = dat & ((1 << 6) - 1)
    
    field['now_turn'] = f.read(1)
    field['final_turn'] = f.read(1)

    field['agent'] = [[[0] * 2] * 2] * 2

    for side in range(2):
        for agent in range(2):
            dat = f.read(1)
            field['agent'][side][agent][0] = dat >> 4
            field['agent'][side][agent][1] = dat & ((1 << 4) - 1)
    
    field_list.append(field)

    return True

def read_binary():

    file = open(const.csv_path, 'rb')

    move_list = []

    while unpack_field(file, move_list):
        pass


    print("hoge")





read_binary()