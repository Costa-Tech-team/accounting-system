set(
  UNIT_TESTS 
  accounts_test
)

set(TESTS_ARGS) 

find_package(Catch2 CONFIG REQUIRED)

add_executable(
  accounts_test
  accounts_test.cpp
  ${CMAKE_SOURCE_DIR}/src/core/account_node.cpp
  ${CMAKE_SOURCE_DIR}/src/core/accounting_entry.cpp
)

foreach(TEST_NAME IN LISTS UNIT_TESTS)
  add_test(${TEST_NAME} ${TEST_NAME} ${TESTS_ARGS})
  target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)  
  target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
endforeach()