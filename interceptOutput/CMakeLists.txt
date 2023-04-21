project(interceptOutput LANGUAGES CXX)

add_executable(retee retee.cpp)
target_compile_options(retee PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(retee PRIVATE Dyninst::dyninstAPI)
