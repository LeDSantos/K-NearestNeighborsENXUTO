#!/bin/bash

echo "teste de MULTIPLICACAO DE MAT"
cd emb_exemplo
arm-linux-gnueabi-gcc -static testeMM.c -o testeMM
cd ..
build/ARM/gem5.opt emb/st_sim.py -c emb_exemplo/testeMM --cpu ARM_A15
cd m5out
cat stats.txt
