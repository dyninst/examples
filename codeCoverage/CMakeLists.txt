cmake_minimum_required(VERSION 3.4.0)
project(codeCoverage LANGUAGES CXX)

add_executable(codeCoverage codeCoverage.C)

target_include_directories(codeCoverage PRIVATE ${DYNINST_INCLUDE_DIR})

target_link_libraries(codeCoverage dyninstAPI)
