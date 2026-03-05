#!/usr/bin/env python3
from random import random


def generate_matrix(N):
    a = [0] * N*N
    for i in range(N):
        for j in range(N):
            if (j > i): # над главной диагональю
                a[i*N + j] = random() # returns x: 0 <= x < 1
            elif (j < i): # под главной диагональю
                a[i*N + j] = a[j*N + i] 
            else:  # на диагонали
                a[i*N + j] = N
    return a

N = 10
matrix = generate_matrix(N)

# сохранить сгенерированную матрицу
with open('matrix.txt', 'w') as f:
    f.write(f"{N}\n")
    for i in range(N):
        for j in range(N):
            x = matrix[i*N + j]
            f.write(f"{x:.2f} ") 
        f.write('\n')