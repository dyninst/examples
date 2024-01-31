project(dyninstAPI LANGUAGES CXX)

add_executable(wrapFunction wrapFunction.cpp)
target_compile_options(wrapFunction PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(wrapFunction Dyninst::dyninstAPI)
