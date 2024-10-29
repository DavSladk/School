# Paralelní programování na GPU (PCG 2021)
# Projekt c. 1 (cuda)
# Login: xsladk07

LIBS=-lhdf5

.PHONY: all clean

all: gen

gen: gen.cpp h5Helper.cpp
	g++ gen.cpp h5Helper.cpp ${LIBS} -o gen

clean:
	rm -f *.o nbody

step0:
	./gen 25600 25600Input.h5 
	./gen 28160 28160Input.h5
	./gen 30720 30720Input.h5
	./gen 33280 33280Input.h5
	./gen 35840 35840Input.h5
	./gen 38400 38400Input.h5
	./gen 40960 40960Input.h5
	./gen 43520 43520Input.h5
	./gen 46080 46080Input.h5
	./gen 48640 48640Input.h5
	./gen 51200 51200Input.h5
	./gen 53760 53760Input.h5
	./gen 56320 56320Input.h5
	./gen 58880 58880Input.h5
	./gen 61440 61440Input.h5
	./gen 64000 64000Input.h5
