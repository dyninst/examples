cmake_minimum_required(VERSION 3.4.0)
project(instrumentAFunction LANGUAGES CXX)

add_executable(instrumenting_a_function instrumenting_a_function.cpp)
target_include_directories(instrumenting_a_function PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(instrumenting_a_function dyninstAPI ${DYNINST_Boost_LIBRARIES})

install(TARGETS InterestingProgram 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}
    )

