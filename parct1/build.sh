#!/bin/bash

SOURCES=(src/linear/*.c)

PROG_NAME="prog.out"

# set the compliler
COMPILER=mpicc

echo :: compile [${SOURCES[*]}] with $COMPILER 

#compile using the compiler
$COMPILER ${SOURCES[*]} -o $PROG_NAME && chmod +x $PROG_NAME



