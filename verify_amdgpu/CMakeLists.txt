project(InstructionAPI LANGUAGES CXX)

add_executable(verify_amdgpu verify_amdgpu.cpp)
target_compile_options(verify_amdgpu PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(verify_amdgpu PRIVATE Dyninst::instructionAPI Dyninst::parseAPI)
install(FILES 1-hipv4-amdgcn-amd-amdhsa--gfx90a DESTINATION ${PROJECT_BINARY_DIR})


