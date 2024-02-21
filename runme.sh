clang++ -O2 partition.cc extswap.cc -L${CppBenchmark}/bin -L${CppBenchmark}/build/modules -lcppbenchmark -lHdrHistogram -lcpp-optparse
clang++ -O2 partition.cc extswap.cc -I${CppBenchmark}/include -L${CppBenchmark}/bin -L${CppBenchmark}/temp/modules -lcppbenchmark -lHdrHistogram -lcpp-optparse
clang++ -O2 virtual-1.cc virtual-2.cc -I${CppBenchmark}/include -L${CppBenchmark}/bin -L${CppBenchmark}/temp/modules -lcppbenchmark -lHdrHistogram -lcpp-optparse
./a.out -q -o csv | tee partition.csv
