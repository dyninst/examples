cmake_minimum_required(VERSION 3.4.0)
project(codeCoverage LANGUAGES CXX)

# The main code coverage tool
add_executable(instrumenting_a_function instrumenting_a_function.cpp)
target_include_directories(instrumenting_a_function PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(instrumenting_a_function dyninstAPI)


# An example binary to be manipulated by codeCoverage
add_executable(InterestingProgram ${EXAMPLE_ROOT}/common/mutatees/InterestingProgram.cpp)
#target_link_libraries(testcc Inst external)

# Some builds of gcc have PIE enabled by default. Dyninst doesn't handle them
# correctly, so disable this
#if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#  target_compile_options(testcc PRIVATE "-fno-pie")
#  target_link_options(testcc PRIVATE "-fno-pie")
#endif()
