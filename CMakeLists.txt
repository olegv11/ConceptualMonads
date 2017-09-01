cmake_minimum_required(VERSION 3.2)
project(ConceptualMonads)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -std=c++1z")

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB testsources ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(ConceptualMonadsTest ${headers} ${testsources})
target_link_libraries(ConceptualMonadsTest ${GTEST_BOTH_LIBRARIES} pthread)

add_test(NAME ConceptualMonadsTests COMMAND ConceptualMonadsTest -v)
