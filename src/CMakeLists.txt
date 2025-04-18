cmake_minimum_required(VERSION 3.28)

include(GoogleTest)

# solution1

    add_library(solution1
        solution1.cpp
    )

    target_include_directories(solution1
    PUBLIC
        .
    )

    target_compile_features(solution1
    PUBLIC
        cxx_std_98
    )

# solutions

    add_library(solutions
        solution2.cpp
        solution3.cpp
        solution4.cpp
        solution5.cpp
    )

    target_include_directories(solutions
    PUBLIC
        .
    )

    target_compile_features(solutions
    PUBLIC
        cxx_std_23
    )

    target_compile_definitions(solutions
        PRIVATE DEBUG_HASH
    )

# quiz

    add_executable(quiz
        main.cpp
    )

    target_compile_features(quiz
    PRIVATE
        cxx_std_23
    )

    target_include_directories(quiz
    PRIVATE
    .
    )

    install(
        TARGETS quiz
        RUNTIME DESTINATION bin COMPONENT Runtime
    ) 

# solution 1 Test

    add_executable(solution1test
        solution1.t.cpp
    )

    target_include_directories(solution1test
    PRIVATE
        .
    )

    target_link_libraries(solution1test
        gtest
        gtest_main
        solution1
    )

    target_compile_features(solution1test
    PUBLIC
        cxx_std_98  
    )

    gtest_discover_tests(solution1test)

# solution 2 Test

    add_executable(solution2test
        solution2.t.cpp
    )

    target_include_directories(solution2test
    PRIVATE
        .
    )

    target_link_libraries(solution2test
        gtest
        gtest_main
        solutions
    )

    target_compile_features(solution2test
    PUBLIC
        cxx_std_23  
    )

    gtest_discover_tests(solution2test)

# solution 3 Test

    add_executable(solution3test
        solution3.t.cpp
    )

    target_include_directories(solution3test
    PRIVATE
        .
    )

    target_link_libraries(solution3test
        gtest
        gtest_main
        solutions
    )

    target_compile_features(solution3test
    PUBLIC
        cxx_std_23
    )

    gtest_discover_tests(solution3test)

# solution 4 Test

    add_executable(solution4test
        solution4.t.cpp
    )

    target_include_directories(solution4test
    PRIVATE
        .
    )

    target_link_libraries(solution4test
        gtest
        gtest_main
        solutions
    )

    target_compile_features(solution4test
    PUBLIC
        cxx_std_23  
    )

    gtest_discover_tests(solution4test)

# solution 5 Test

    add_executable(solution5test
        solution5.t.cpp
    )

    target_include_directories(solution5test
    PRIVATE
        .
    )

    target_link_libraries(solution5test
        gtest
        gtest_main
        gmock
        gmock_main
        solutions
    )

    target_compile_features(solution5test
    PUBLIC
        cxx_std_23  
    )

    gtest_discover_tests(solution5test)

