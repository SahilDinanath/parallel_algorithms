import random

file = open("input_2_3.txt","w")

for i in range(2**3):
    line = str(random.randint(0,9))
    file.write(line)

file.close()