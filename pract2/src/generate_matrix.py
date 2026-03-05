#!/usr/bin/env python3
from random import random
from math import pi, sin

# Au = b
def symmetric(N):
    a = [0] * N*N
    for i in range(N):
        for j in range(N):
            if (j > i): # над главной диагональю
                a[i*N + j] = random() # returns x: 0 <= x < 1
            elif (j < i): # под главной диагональю
                a[i*N + j] = a[j*N + i] 
            else:  # на диагонали
                a[i*N + j] = N  
    
    u = [sin(2 * pi * i / N) for i in range(1, N+1)]
    b = [a[i] * u[i] for i in range(N) ]
    return a, b

# Ax = b, x should be (1,1,1,1,1,1,...)
def test(N):
    a = [2 if i == j else 1 for i in range(N) for j in range(N)]
    b = [N+1] * N
    return a,b

N = 10
#matrix, b = symmetric(N)
matrix, b = test(N)

# сохранить сгенерированную матрицу и вектор b (правая часть)
with open('matrix.txt', 'w') as f:
    f.write(f"{N}\n")
    for i in range(N):
        for j in range(N):
            x = matrix[i*N + j]
            f.write(f"{x} ") 
        f.write('\n')

    for num in b:
        f.write(f"{num} ")


    