project(dataflowapi)

add_library(slicing SHARED slicing.cpp)
target_compile_options(slicing PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(slicing Dyninst::parseAPI)

add_library(symbolic_evaluation SHARED symbolic_evaluation.cpp)
target_compile_options(symbolic_evaluation PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(symbolic_evaluation Dyninst::parseAPI)

add_library(liveness SHARED liveness.cpp)
target_compile_options(liveness PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(liveness Dyninst::parseAPI)

add_library(stack_analysis SHARED liveness.cpp)
target_compile_options(stack_analysis PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(stack_analysis Dyninst::parseAPI)
