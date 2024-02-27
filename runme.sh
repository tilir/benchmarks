#!/bin/bash

export CppBenchmark=/mnt/c/research/CppBenchmark/
export OPTIONS="-I${CppBenchmark}/include -L${CppBenchmark}/bin -L${CppBenchmark}/build/modules -L${CppBenchmark}/temp/modules"
export LIBS="-lcppbenchmark -lHdrHistogram -lcpp-optparse"

clang++ -O2 ./inline/partition.cc ./inline/extswap.cc ${OPTIONS} ${LIBS}
./a.out -q -o csv | tee partition.csv
