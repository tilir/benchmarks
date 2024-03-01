#!/bin/bash

export CppBenchmark=/mnt/c/research/CppBenchmark/
export OPTIONS="-I${CppBenchmark}/include -L${CppBenchmark}/bin -L${CppBenchmark}/build/modules -L${CppBenchmark}/temp/modules"
export LIBS="-lcppbenchmark -lHdrHistogram -lcpp-optparse"

clang++ -O2 ./inline/partition.cc ./inline/extswap.cc ${OPTIONS} ${LIBS}
./a.out -q -o csv | tee partition.csv
clang++ -O2 ./virtual-overhead/virtual-1.cc ./virtual-overhead/virtual-2.cc ${OPTIONS} ${LIBS}
./a.out -q -o csv | tee virtual.csv
clang++ -O2 ./virtual-overhead/virtual-shuffle-1.cc ./virtual-overhead/virtual-shuffle-2.cc ${OPTIONS} ${LIBS}
./a.out -q -o csv | tee virtual-shuffle.csv




clang++ -O2 -std=c++20 ./ranges-projector/projector.cc ${OPTIONS} ${LIBS}
./a.out -q -o csv | tee ranges-proj.csv
