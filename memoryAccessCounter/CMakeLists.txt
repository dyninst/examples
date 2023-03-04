project(memoryAccessCounter LANGUAGES CXX)

add_executable(counter counter.cpp)
target_compile_options(counter PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(counter Dyninst::dyninstAPI)

install(TARGETS InterestingProgram  RUNTIME DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(memoryAccessCounter DEPENDS counter)
