#!/bin/bash

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o oems oems.c

#vyrobeni souboru s random cisly, usmrceni vystupu
dd if=/dev/random bs=1 count=8 of=numbers >/dev/null 2>&1

#spusteni
mpirun --prefix /usr/local/share/OpenMPI -oversubscribe -np 20 oems

#uklid
rm -f oems numbers