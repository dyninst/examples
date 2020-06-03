cmake_minimum_required(VERSION 3.4.0)
project(codeCoverage LANGUAGES CXX)

# The main code coverage tool
add_executable(code_coverage code_coverage.C)
target_include_directories(code_coverage PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(code_coverage dyninstAPI)

# An example instrumentation library
add_library(Inst SHARED Inst.C)

# An example external library used by testcc
add_library(external SHARED external.C)

# An example binary to be manipulated by codeCoverage
add_executable(testcc testcc.C)
target_link_libraries(testcc Inst external)

# Some builds of gcc have PIE enabled by default. Dyninst doesn't handle them
# correctly, so disable this
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(testcc PRIVATE "-fno-pie")
  target_link_options(testcc PRIVATE "-fno-pie")
endif()

# A dummy target to allow building this example by itself
add_custom_target(codeCoverage DEPENDS code_coverage testcc)