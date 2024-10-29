#/**
# * @file      Makefile
# *
# * @author    David Sladký \n
# *            xsladk07@stud.fit.vutbr.cz
# *
# * @brief     PCG Assignment 2
# *            N-Body simulation in ACC - Makefile
# *
# * @version   2022
# *
# * @date      11 November  2020, 11:22 (created) \n
# * @date      04 December  2020, 10:57 (revised) \n
# *
# */

LIBS=-lhdf5

.PHONY: all clean

all: gen

gen: gen.cpp h5Helper.cpp
	g++ gen.cpp h5Helper.cpp ${LIBS} -o gen

generate:
	./gen 1024  data/1024.h5
	./gen 2048  data/2048.h5
	./gen 3072  data/3072.h5
	./gen 4096  data/4096.h5
	./gen 5120  data/5120.h5
	./gen 6144  data/6144.h5
	./gen 7168  data/7168.h5
	./gen 8192  data/8192.h5
	./gen 9216  data/9216.h5
	./gen 10240 data/10240.h5
	./gen 11264 data/11264.h5
	./gen 12288 data/12288.h5
	./gen 13312 data/13312.h5
	./gen 14336 data/14336.h5
	./gen 15360 data/15360.h5
	./gen 16384 data/16384.h5
	./gen 17408 data/17408.h5
	./gen 18432 data/18432.h5
	./gen 19456 data/19456.h5
	./gen 20480 data/20480.h5

clean:
	rm -f *.o gen
