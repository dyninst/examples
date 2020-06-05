cmake_minimum_required(VERSION 3.4.0)
project(instrumentMemoryAccess LANGUAGES CXX)

add_executable(instrumenting_memory_access instrumenting_memory_access.cpp)
target_include_directories(instrumenting_memory_access PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(instrumenting_memory_access dyninstAPI instructionAPI)

install(TARGETS InterestingProgram 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}
    )

