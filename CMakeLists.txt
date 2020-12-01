    cmake_minimum_required(VERSION 3.10)

    project(pagrnd_cache)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    find_package(Boost)
    find_package(GTest)

    include_directories(
        include/
        include/models/
        include/replacer/
        include/data_handlers/
        include/addr_handlers/
    )

    file(GLOB DEMO_SRCS
        "demo/demo.cpp"
    )

    file(GLOB TEST_SRCS
        "tests/test_cache.cpp"
    )

    set(CACHE_TEST cache_test)
    set(CACHE_DEMO cache_demo)

    add_executable(${CACHE_TEST} ${TEST_SRCS})
    target_link_libraries(${CACHE_TEST} PUBLIC gtest gtest_main pthread)
    target_compile_options(${CACHE_TEST} PUBLIC -Wall -Werror -Wextra)

    gtest_discover_tests(${CACHE_TEST})

    enable_testing()

    add_executable(${CACHE_DEMO} ${DEMO_SRCS})
    target_compile_options(${CACHE_DEMO} PUBLIC -O3 -Wall -Werror -Wextra)