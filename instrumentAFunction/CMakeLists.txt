project(instrumentAFunction LANGUAGES CXX)

add_executable(instrumenting_a_function instrumenting_a_function.cpp)
target_compile_options(instrumenting_a_function PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(instrumenting_a_function Dyninst::dyninstAPI)

install(TARGETS InterestingProgram  RUNTIME DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(instrumentAFunction DEPENDS instrumenting_a_function)