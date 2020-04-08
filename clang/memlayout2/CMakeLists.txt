cmake_minimum_required(VERSION 3.13)

project(memlayout CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(AddLLVM)
include(AddClang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
  src/main.cc
  src/memlayout.cc
  src/func_temp_arg_type_finder.cc
  src/namespace_struct_finder.cc)

set(CLANG_LIBS
  clangTooling
  clangDriver
  clangFrontend
  clangSerialization
  clangParse
  clangSema
  clangEdit
  clangLex
  clangAnalysis
  clangDynamicASTMatchers
  clangASTMatchers
  clangAST
  clangBasic
  LLVM)

target_link_libraries(${PROJECT_NAME}
  ${CLANG_LIBS}
  )
