cmake_minimum_required(VERSION 3.11)
project("benchmarks")

enable_testing()
set(CSVS ${CMAKE_BINARY_DIR}/CSVS)
file(MAKE_DIRECTORY ${CSVS})

find_package(cppbenchmark REQUIRED)

add_custom_target(runall)

add_subdirectory(coro-fibs)
add_subdirectory(excret)
add_subdirectory(inline)
add_subdirectory(noexcept-qsort)
add_subdirectory(ranges-filter)
add_subdirectory(ranges-projector)
add_subdirectory(virtual-inherit)
add_subdirectory(virtual-inline)
add_subdirectory(virtual-overhead)
