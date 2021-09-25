cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_STANDARD "99")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -Og")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

project(cuutest VERSION 0.1.0 LANGUAGES C)

add_library(cuutest cuutest.c)
target_include_directories(cuutest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CUUTEST_EXAMPLE)
    add_executable(example example.c)
    target_link_libraries(example cuutest)

    enable_testing()
    add_test(NAME cuutest_example COMMAND $<TARGET_FILE:example>)
endif(CUUTEST_EXAMPLE)
