cmake_minimum_required(VERSION 3.4.0)
project(binaryAnalysis LANGUAGES CXX)

# The main code coverage tool
add_executable(binary_analysis binary_analysis.cpp)
target_include_directories(binary_analysis PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(binary_analysis dyninstAPI instructionAPI)


# An example binary to be manipulated by codeCoverage
#add_library(InterestingProgram ${EXAMPLE_ROOT}/common/mutatees/InterestingProgram.cpp)

#target_link_libraries(testcc Inst external)
install(TARGETS InterestingProgram 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}
    )
# Some builds of gcc have PIE enabled by default. Dyninst doesn't handle them
# correctly, so disable this
#if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#  target_compile_options(testcc PRIVATE "-fno-pie")
#  target_link_options(testcc PRIVATE "-fno-pie")
#endif()
