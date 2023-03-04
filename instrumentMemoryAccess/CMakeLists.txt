project(instrumentMemoryAccess LANGUAGES CXX)

add_executable(instrumenting_memory_access instrumenting_memory_access.cpp)
target_compile_options(instrumenting_memory_access PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(instrumenting_memory_access Dyninst::dyninstAPI)

install(TARGETS InterestingProgram  RUNTIME DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(instrumentMemoryAccess DEPENDS instrumenting_memory_access)
