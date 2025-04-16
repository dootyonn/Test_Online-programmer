cmake_minimum_required(VERSION 3.28)

include(GoogleTest)

# quiz

    add_executable(quiz
        main.cpp
    )

    target_compile_features(quiz
    PUBLIC
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

# question 1

    add_executable(question1
        question1.t.cpp
    )

    target_include_directories(question1
    PRIVATE
        .
    )

    target_link_libraries(question1
        gtest
        gtest_main
    )

    target_compile_features(question1
    PUBLIC
        cxx_std_98  
    )

    gtest_discover_tests(question1)

# question 2

    add_executable(question2
        question2.t.cpp
    )

    target_include_directories(question2
    PRIVATE
        .
    )

    target_link_libraries(question2
        gtest
        gtest_main
    )

    target_compile_features(question2
    PUBLIC
        cxx_std_23  
    )

    gtest_discover_tests(question2)

# question 3

    add_executable(question3
        question3.t.cpp
    )

    target_include_directories(question3
    PRIVATE
        .
    )

    target_link_libraries(question3
        gtest
        gtest_main
    )

    target_compile_features(question3
    PUBLIC
        cxx_std_23
    )

    gtest_discover_tests(question3)

# question 4

    add_executable(question4
        question4.t.cpp
    )

    target_include_directories(question4
    PRIVATE
        .
    )

    target_link_libraries(question4
        gtest
        gtest_main
    )

    target_compile_features(question4
    PUBLIC
        cxx_std_23  
    )

    gtest_discover_tests(question4)

# question 5

    add_executable(question5
        question5.t.cpp
    )

    target_include_directories(question5
    PRIVATE
        .
    )

    target_link_libraries(question5
        gtest
        gtest_main
    )

    target_compile_features(question5
    PUBLIC
        cxx_std_23  
    )

    gtest_discover_tests(question5)

