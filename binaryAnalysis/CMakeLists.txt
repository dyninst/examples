cmake_minimum_required(VERSION 3.4.0)
project(binaryAnalysis LANGUAGES CXX)

add_executable(binary_analysis binary_analysis.cpp)
target_include_directories(binary_analysis PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(binary_analysis dyninstAPI instructionAPI)

install(TARGETS InterestingProgram 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}
    )

