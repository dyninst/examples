project(unstrip LANGUAGES CXX)

add_executable(disassemble disassemble.cpp)
target_compile_options(disassemble PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(disassemble Dyninst::dyninstAPI)

add_executable(unknown_instruction unknown_instruction.cpp)
target_compile_options(unknown_instruction PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(unknown_instruction Dyninst::dyninstAPI)
