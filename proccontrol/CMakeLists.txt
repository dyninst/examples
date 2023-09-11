project(proccontrol LANGUAGES CXX)

add_executable(callback callback.cpp)
target_compile_options(callback PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(callback Dyninst::pcontrol)
