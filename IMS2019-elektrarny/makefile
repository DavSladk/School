COMPILER=g++

FLAGS= -std=c++11 -lsimlib -lm

.PHONY: run clean

main: main.cpp
	$(COMPILER) -o $@ $< $(FLAGS)

run:
	./main ${ARGS}

clean:
	rm -f main