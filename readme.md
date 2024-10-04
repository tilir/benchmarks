# Benchmarks for Yadro C++ meetup talk

This repo consists of Yadro C++ talk benchmarks.

## Building and running

We are using CppBenchmark via Conan repo. To guarantee clean conan setup please use venv. See full build sequence below.

First clone repo and perform other actions from repo source folder.

```
git clone https://github.com/tilir/benchmarks.git
cd benchmarks
```

This project uses poetry. Please install poetry according to official installation instructions.
Next, install dependencies using poetry and activate venv:

```
poetry install --no-root
poetry shell
```

Note: if you never had conan earlier on your machine, please also run

```
conan profile detect
```

If you have custom compiler be sure it is inside profile.

Now you are ready for main build and run. Conan will download all dependencies for you.

```
conan install conanfile.txt --build=missing
cmake -S . -B build/Release --toolchain build/Release/generators/conan_toolchain.cmake  -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/Release --target test --parallel 1
```

This will take some time, be patient. Benchmarking results will be created in the build/Release/CSVS folder. 

You are welcome to create MR in results folder with your architecture and name it will help my talk!



## Troubleshooting

### error: 'uint64_t' does not name a type during `conan install` phase
```bash
cppbench_version=$(grep -a cppbenchmark conanfile.txt)
cppbench_path=$(conan cache path $cppbench_version)
cppbench_header_path=$cppbench_path/../s/src/include/benchmark/system.h
sed -i 's/<string>/<string>\n#include <cstdint>/' $cppbench_header_path
```

Note: normally it is highly discouraged to modify sources inside conan cache, this is just a temporary workaround
