#!/bin/bash

mpicc common.c -o common.out
mpicc dotdot.c -o dotdot.out
gcc oneproc.c -o oneproc.out
