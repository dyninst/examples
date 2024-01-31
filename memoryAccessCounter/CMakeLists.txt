project(memoryAccessCounter LANGUAGES CXX)

add_executable(counter counter.cpp)
target_compile_options(counter PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(counter PRIVATE Dyninst::dyninstAPI)

install(TARGETS InterestingProgram RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
