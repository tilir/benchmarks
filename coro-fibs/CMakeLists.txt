SET(TNAME coro-fibs)

add_executable(${TNAME} corofibs.cc sumfib_module.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)

file(TO_NATIVE_PATH "${CSVS}/${TNAME}.csv" CSV_OUT)

if (WIN32)
# cannot use cmd here because of some strange things in path: https://gitlab.kitware.com/cmake/cmake/-/issues/17067
  add_test(
    NAME TEST_${TNAME}
    COMMAND powershell -command "$<TARGET_FILE:${TNAME}> -q -o csv > ${CSV_OUT}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
else()
  add_test(
    NAME TEST_${TNAME}
    COMMAND sh -c "$<TARGET_FILE:${TNAME}> -q -o csv > ${CSV_OUT}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${TNAME})
