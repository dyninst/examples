project(dataflowapi)

add_library(slicing SHARED slicing.cpp)
target_compile_options(slicing PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(slicing Dyninst::parseAPI)
