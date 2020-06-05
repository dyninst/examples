cmake_minimum_required(VERSION 3.4.0)
project(maxArg LANGUAGES CXX)

add_executable(maxArg maxArg.cpp)
target_include_directories(maxArg PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(maxArg dyninstAPI)
