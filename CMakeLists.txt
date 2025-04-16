cmake_minimum_required(VERSION 3.28)

project(
  quiz
  VERSION 1.0.0
  LANGUAGES CXX
)

enable_testing()
include(CTest)

find_package(GTest REQUIRED)

add_subdirectory(src)
