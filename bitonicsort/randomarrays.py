import random

file = open("input_2_20.txt", "w")

def generate_bitonic_sequence(n):
    seq = [random.randint(0, 9) for _ in range(n)]
    seq.sort()  # Sorting the sequence in ascending order
    mid = n // 2
    seq[mid:] = reversed(seq[mid:])  # Reversing the second half of the sequence
    return seq

n = 2 ** 20  # Size of the bitonic sequence
bitonic_seq = generate_bitonic_sequence(n)

for num in bitonic_seq:
    line = str(num)
    file.write(line)

file.close()