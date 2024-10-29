.PHONY: clean prep build run zip all

TO_ZIP=\
CMakeLists.txt\
CZ.freq\
EN.freq\
Makefile\
main.cpp\
kry.cpp\
kry.h\
dokumentace.md

TO_CLEAN=\
build/\
175082.zip\
kry

all: clean prep build

prep:
	mkdir -p build
	cd build && cmake ..

build:
	cd build && cmake --build . && cp kry ../kry

clean:
	rm -rf $(TO_CLEAN)

zip:
	cp README.md dokumentace.md
	zip 175082.zip $(TO_ZIP)
	rm dokumentace.md
