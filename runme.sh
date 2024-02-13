clang++ -O2 partition.cc extswap.cc -L${CppBenchmark}/bin -L${CppBenchmark}/build/modules -lcppbenchmark -lHdrHistogram -lcpp-optparse
./a.out -q -o csv | tee partition.csv
