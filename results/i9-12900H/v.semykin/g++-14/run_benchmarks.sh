#!/bin/bash

declare -A benchmarks=(
    # ["coro-fibs"]="coro-fibs/coro-fibs"
    # ["exc_ret"]="excret/exc_ret"
    # ["exc_ret_noexc"]="excret/exc_ret_noexc"
    # ["extswap"]="inline/extswap"
    # ["exc_partition"]="noexcept-qsort/exc_partition"
    # ["exc_qsort"]="noexcept-qsort/exc_qsort"
    # ["filter"]="ranges-filter/filter"
    # ["projector"]="ranges-projector/projector"
    ["virtinh"]="virtual-inherit/virtinh"
    # ["virtinl"]="virtual-inline/virtinl"
    # ["virtual"]="virtual-overhead/virtual"
    # ["virtual-shuffle"]="virtual-overhead/virtual-shuffle"
)

base_dir="../../../../build/Release"

for benchmark in "${!benchmarks[@]}"; do
    exec_path="$base_dir/${benchmarks[$benchmark]}"

    if [[ -f $exec_path ]]; then
        $exec_path -o csv | sed -r "s/\x1b\[[0-9;]*m//g" > "./$benchmark.csv"
        echo "Benchmark $benchmark completed successfully."
    else
        echo "Executable for $benchmark not found!"
    fi
done
