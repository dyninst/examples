project(CFGraph LANGUAGES CXX)

add_executable(CFGraph CFGraph.cpp)
target_compile_options(CFGraph PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(CFGraph PRIVATE Dyninst::dyninstAPI)
