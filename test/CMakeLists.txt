cmake_minimum_required(VERSION 3.0.0)
project(GenADTests VERSION 0.1.0)

include(CTest)
enable_testing()

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(GenADTests test.cpp)

target_link_libraries(GenADTests ${GTEST_LIBRARIES} pthread)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
