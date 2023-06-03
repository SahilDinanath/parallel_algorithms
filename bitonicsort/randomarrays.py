import random

file = open("input_2_16.txt","w")

for i in range(2**16):
    line = str(random.randint(0,9))
    file.write(line)

file.close()
