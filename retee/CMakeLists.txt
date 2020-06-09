cmake_minimum_required(VERSION 3.4.0)
project(retee LANGUAGES CXX)

add_executable(retee retee.cpp)
target_include_directories(retee PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(retee dyninstAPI)
