project(InstructionAPI LANGUAGES CXX)

add_executable(stateless_visitor stateless_visitor.cpp)
target_compile_options(stateless_visitor PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(stateless_visitor PRIVATE Dyninst::instructionAPI)

add_executable(statefull_visitor statefull_visitor.cpp)
target_compile_options(statefull_visitor PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(statefull_visitor PRIVATE Dyninst::instructionAPI)
