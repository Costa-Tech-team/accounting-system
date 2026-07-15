cmake_minimum_required(VERSION 4.0.0)
project(accountable_system)

option(ENABLE_TESTING "Enable testing" ${PROJECT_IS_TOP_LEVEL})
set(CMAKE_CXX_STANDARD 23)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(src)