#!/bin/bash

#spocteni poctu potrebnych procesu
n=${#1}

#jenom -1, protoze chci extra procesor 0 na praci okolo
p=$(($n*2-1))

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o pro pro.cpp

#spusteni
mpirun --prefix /usr/local/share/OpenMPI -oversubscribe -np $p pro $1

#uklid
rm -f pro