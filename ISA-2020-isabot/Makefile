.PHONY: all clean default_target test build run cmake pack

default_target: all

all: clean cmake build

cmake:
	mkdir build && cd build && cmake ..

build:
	cd build && cmake --build . && mv isabot ../isabot

run:
	./build/isabot

clean:
	rm -fr ./build isabot xsladk07.tar

test:
	echo "no tests"

pack:
	tar -cf xsladk07.tar README.md Makefile CMakeLists.txt manual.pdf src/* headers/*