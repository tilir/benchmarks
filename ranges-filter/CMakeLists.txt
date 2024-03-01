SET(TNAME filter)

add_executable(${TNAME} filter.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)

add_test(
  NAME TEST_${TNAME}
  COMMAND ${TNAME} -q -o csv > ${CMAKE_BINARY_DIR}/CSVS/${TNAME}.csv)
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${TNAME})

