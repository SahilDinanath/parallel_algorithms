import random

with open('/home/darsh/PC/PC_assignment_2023/bitonicsort/tests/input.txt', 'r') as input_file, open('arrays.txt', 'w') as output_file:
    for line in input_file:
        count = int(line.strip())
        array = [random.randint(0, 100) for _ in range(count)]
        output_file.write(' '.join(map(str, array)) + '\n')
