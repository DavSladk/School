.PHONY: compile run zip clean test

compile: src.pl
	swipl -q -g main -o flp21-log -c src.pl

run:
	./flp21-log < tests/test01.in

clean:
	rm -f flp21-log flp-log-xsladk07.zip

zip: clean
	zip flp-log-xsladk07.zip src.pl makefile README.md TIMES.md tests/*.in tests/test.sh

test: compile
	bash tests/test.sh
