project(DynC LANGUAGES CXX)

add_executable(dync_mutatee mutatee.cpp)
target_compile_options(dync_mutatee PRIVATE ${EXAMPLES_WARNING_FLAGS})

add_executable(dync_mutator mutator.cpp)
target_compile_options(dync_mutator PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(dync_mutator PRIVATE Dyninst::dynC_API
                                           Dyninst::dyninstAPI)

install(FILES testStatements testStatements2 DESTINATION ${PROJECT_BINARY_DIR})
