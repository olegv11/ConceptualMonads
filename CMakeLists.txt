cmake_minimum_required(VERSION 3.8)
project(ConceptualMonads)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB testsources ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(ConceptualMonadsTest ${headers} ${testsources})
target_link_libraries(ConceptualMonadsTest ${GTEST_BOTH_LIBRARIES} pthread)

add_test(NAME ConceptualMonadsTests COMMAND ConceptualMonadsTest -v)
