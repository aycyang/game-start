cmake_minimum_required(VERSION 3.14)

project(Log VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME}
  log.cc
)

add_executable(${PROJECT_NAME}Test
  log_test.cc
  log.cc
)

enable_testing()
add_test(NAME ${PROJECT_NAME}Test COMMAND ${PROJECT_NAME}Test)
