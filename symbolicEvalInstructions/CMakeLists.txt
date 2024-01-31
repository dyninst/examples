project(symbolicEvalInstructions)

add_executable(sym_inst symbolic_instruction_evaluation.cpp)
target_compile_options(sym_inst PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_compile_options(sym_inst PRIVATE "-g")
target_link_libraries(sym_inst PRIVATE Dyninst::parseAPI Dyninst::dyninstAPI
                                       Dyninst::instructionAPI)
