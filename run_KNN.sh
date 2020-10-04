#!/bin/bash

echo "Alg K-NearestNeighborsENXUTO"
cd emb_exemplo
cd K-NearestNeighborsENXUTO
ls
arm-linux-gnueabi-gcc -static mainTUDO.c -lm -o CrossFold_Kvizinhos
cd ..
cd ..
build/ARM/gem5.opt emb/st_sim.py -c emb_exemplo/K-NearestNeighborsENXUTO/CrossFold_Kvizinhos --cpu ARM_A15
cd m5out
cat stats.txt

