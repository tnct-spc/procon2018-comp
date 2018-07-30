import const

def make_field(f):

    field = {}

    siz = f.read(1)

    if siz == '':
        return False
    
    field['siz'] = [siz & 7, (siz >> 4) & 7]

    field['state'] = [[0] * siz[1]] * siz[0]
    field['value'] = [[0] * siz[1]] * siz[0]

    for x in range(siz[0]):
        for y in range(size[1]):

    return True

def read_binary():

    file = open('csv_path', 'rb')

    move_list = []

    while make_field(f, move_list):
        pass


    print("hoge")





read_binary()