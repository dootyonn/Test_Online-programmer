cmake_minimum_required(VERSION 3.28)

project(quiz
  VERSION 1.0.0
  LANGUAGES CXX
)

enable_testing()
include(CTest)

find_package(GTest REQUIRED)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list(
        APPEND CMAKE_CXX_CPPCHECK
            "--check-level=exhaustive"
            "--enable=warning"
            "--library=googletest"
            "--inconclusive"
            "--force"
            "--inline-suppr"
            "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )
endif()

add_subdirectory(src)
