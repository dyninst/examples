project(stackwalker LANGUAGES CXX)

add_executable(this_thread this_thread.cpp)
target_compile_options(this_thread PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(this_thread Dyninst::stackwalk)
