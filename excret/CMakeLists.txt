# exception vs ret code bmk when exception happened

SET(TNAME exc_ret)

add_executable(${TNAME} exc_ret_1.cc exc_ret_2.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)

add_test(
  NAME TEST_${TNAME}
  COMMAND sh -c "./${TNAME} -q -o csv > ${CSVS}/${TNAME}.csv"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${TNAME})

# same benchmark, version when no exception happened

SET(TNAME exc_ret_noexc)

add_executable(${TNAME} exc_ret_1.cc exc_ret_2.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)
target_compile_definitions(${TNAME} PUBLIC NOEXC)

add_test(
  NAME TEST_${TNAME}
  COMMAND sh -c "./${TNAME} -q -o csv > ${CSVS}/${TNAME}.csv"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${TNAME})
