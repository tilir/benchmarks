SET(TNAME coro-fibs)

add_executable(${TNAME} corofibs.cc sumfib_module.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)

add_test(
  NAME TEST_${TNAME}
  COMMAND sh -c "./${TNAME} -q -o csv > ${CSVS}/${TNAME}.csv"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${TNAME})

