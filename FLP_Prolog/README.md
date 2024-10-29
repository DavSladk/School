Points: 8/8

* `make` - compiles the program
* `make compile` - compiles the program
* `make run` - compiles the program and runs it with `test01` and output to stdout
* `make clean` - cleans unwanted files
* `make zip` - cleans unwanted files and make archive for submission
* `make test` - compiles the program and runs `tests/test.sh`

## Description
Graph is loaded from stdin in format defined in assignment. Any line (edge) not satisfying this format is ignored. States of every edge get sorted and then the whole graph is sorted. This way any duplicate edges get removed.

The program generates all subsets of the set of the loaded edges (hence big memory requirements in `test07` and `test09`). After that a check is applied for each subset to determine if it is a spanning tree of the original graph. If it is, it gets printed in required format. The chechking term `isBone` is explained with comments in `src.pl`.

## Tests
Test times in this file were taken on Merlin. They do not include `test07` and `test09` because these graphs take too much memory to calculate and crash on Merlin.

Tests in `TIMES.md` were run on my PC. Project files were stored on Windows filesystem but execution were made via WSL.

To run test script with tests `test07` and `test09`, remove their `.nsfm` (not safe for merlin) extension and uncomment line 5 in `src.pl`, so Prolog has enough memory to execute these tests.

| Test | time 1 | time 2 | time 3 | time 4 | time 5 | time 6 | time 7 | time 8 | time 9 | time 10 | AVG |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| tests/test01.in | 27 ms | 21 ms | 18 ms | 22 ms | 23 ms | 20 ms | 21 ms | 18 ms | 19 ms | 24 ms | 21 ms |
| tests/test02.in | 19 ms | 17 ms | 22 ms | 18 ms | 23 ms | 26 ms | 20 ms | 19 ms | 20 ms | 17 ms | 20 ms |
| tests/test03.in | 20 ms | 20 ms | 18 ms | 20 ms | 24 ms | 21 ms | 17 ms | 17 ms | 23 ms | 20 ms | 20 ms |
| tests/test04.in | 26 ms | 24 ms | 22 ms | 23 ms | 26 ms | 22 ms | 21 ms | 20 ms | 27 ms | 22 ms | 23 ms |
| tests/test05.in | 23 ms | 18 ms | 20 ms | 17 ms | 17 ms | 21 ms | 22 ms | 19 ms | 21 ms | 20 ms | 19 ms |
| tests/test06.in | 27 ms | 21 ms | 19 ms | 17 ms | 18 ms | 22 ms | 21 ms | 20 ms | 21 ms | 24 ms | 21 ms |
| tests/test08.in | 47 ms | 52 ms | 48 ms | 49 ms | 49 ms | 47 ms | 59 ms | 55 ms | 46 ms | 48 ms | 50 ms |
| tests/test10.in | 23 ms | 22 ms | 20 ms | 20 ms | 22 ms | 20 ms | 21 ms | 21 ms | 26 ms | 21 ms | 21 ms |
