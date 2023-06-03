import random

file = open("input_2_18.txt","w")

for i in range(2**18):
    line = str(random.randint(0,9))
    file.write(line)

file.close()
