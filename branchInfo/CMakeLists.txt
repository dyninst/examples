project(branchInfo LANGUAGES CXX)

add_executable(branchInfo
               branchInfo.cpp)

target_compile_options(branchInfo PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(branchInfo
                      Dyninst::dyninstAPI
                      Dyninst::instructionAPI
                      Dyninst::parseAPI
                      Dyninst::symtabAPI
                      Dyninst::common
                      )

