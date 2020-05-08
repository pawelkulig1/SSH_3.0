import os

def calc_time_diff(filename):
    with open(filename) as f:
        data = f.read().splitlines()
    start = float(data[0])
    end = float(data[-1])
    return end - start


files = os.listdir()
avg = 0
counter = 0
for name in files:
    if name.find("out_") != -1:
        diff = calc_time_diff(name)
        print(diff)
        avg += diff
        counter += 1

print("Average:", avg/counter)
        

