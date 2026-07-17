set(
  UNIT_TESTS 
  account_test
)

set(TESTS_ARGS) 

find_package(Catch2 CONFIG REQUIRED)

add_executable(
  account_test
  account_test.cpp
  ${CMAKE_SOURCE_DIR}/src/account.cpp
)

foreach(TEST_NAME IN LISTS UNIT_TESTS)
  add_test(${TEST_NAME} ${TEST_NAME} ${TESTS_ARGS})
  target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)  
  target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
endforeach()