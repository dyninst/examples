project(readGlobalVariables)

add_library(globals SHARED globals.cpp)

add_executable(read_global_variables reader.cpp)
target_compile_options(read_global_variables PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(read_global_variables PRIVATE Dyninst::symtabAPI globals)
