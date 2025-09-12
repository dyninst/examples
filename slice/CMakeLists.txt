project(slice LANGUAGES CXX)

add_executable(slice
               slice.C
)

target_compile_options(slice PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(slice
                      Dyninst::dyninstAPI
                      Dyninst::instructionAPI
                      Dyninst::parseAPI
                      Dyninst::symtabAPI
                      Dyninst::common
                      )

