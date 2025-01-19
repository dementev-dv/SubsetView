cmake_minimum_required(VERSION 3.20)
project(SubsetGenerator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED OLD_STD)
  add_definitions(-DOLD_STD)
endif()

enable_testing()

add_executable(subset SubsetView.cc)

add_test(NAME test1 COMMAND subset ${CMAKE_SOURCE_DIR}/test/input1.txt ${CMAKE_SOURCE_DIR}/test/answer1.txt)
add_test(NAME test2 COMMAND subset ${CMAKE_SOURCE_DIR}/test/input2.txt ${CMAKE_SOURCE_DIR}/test/answer2.txt)
add_test(NAME test3 COMMAND subset ${CMAKE_SOURCE_DIR}/test/input3.txt ${CMAKE_SOURCE_DIR}/test/answer3.txt)
add_test(NAME test4 COMMAND subset ${CMAKE_SOURCE_DIR}/test/input4.txt ${CMAKE_SOURCE_DIR}/test/answer4.txt)
add_test(NAME test5 COMMAND subset ${CMAKE_SOURCE_DIR}/test/input5.txt ${CMAKE_SOURCE_DIR}/test/answer5.txt)
