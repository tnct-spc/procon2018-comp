import csv

def read_csv(csv_path):
    csv_file = open(csv_path, 'r')

    data = csv.reader(csv_file, delimiter=',', skipinitialspace=True)

    ret_data = []
    field_data = []

    for row in data:
        if row[0] == '-1':
            field_data = row[1:]
        else:
            ret_data.append(list(map(float,field_data + row)))
            # 勝敗予測より得点差予測の方がよさそうなので
            ret_data[-1][-1] = 1.0 if (int(ret_data[-1][-1]) > 0) else 0.0

    csv_file.close()

    return ret_data
