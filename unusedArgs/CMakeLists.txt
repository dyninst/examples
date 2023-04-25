project(unusedArgs LANGUAGES CXX)

add_executable(unusedArgs unusedArgs.C)
target_compile_options(unusedArgs PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(unusedArgs PRIVATE Dyninst::dyninstAPI)
