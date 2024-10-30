# exception vs ret code bmk when exception happened

SET(TNAME exc_ret)

add_executable(${TNAME} exc_ret_1.cc exc_ret_2.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)

file(TO_NATIVE_PATH "${CSVS}/${TNAME}.csv" CSV_OUT)

if (WIN32)
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


# same benchmark, version when no exception happened

SET(TNAME exc_ret_noexc)

add_executable(${TNAME} exc_ret_1.cc exc_ret_2.cc)
target_compile_features(${TNAME} PRIVATE cxx_std_20)
target_link_libraries(${TNAME} cppbenchmark::cppbenchmark)
target_compile_definitions(${TNAME} PUBLIC NOEXC)

file(TO_NATIVE_PATH "${CSVS}/${TNAME}.csv" CSV_OUT)

if (WIN32)
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

