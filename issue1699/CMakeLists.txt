project(issue1699 LANGUAGES CXX)

add_executable(issue1699 issue1699.cpp)
target_compile_options(issue1699 PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(issue1699 Dyninst::dyninstAPI)


