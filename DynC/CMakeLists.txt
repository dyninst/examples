project(DynC)

add_executable(dync_mutatee mutatee.cpp)
target_compile_options(dync_mutatee PRIVATE ${EXAMPLES_WARNING_FLAGS})

add_executable(dync_mutator mutator.cpp)
target_compile_options(dync_mutator PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(dync_mutator PRIVATE Dyninst::dynC_API Dyninst::dyninstAPI)

install(FILES testStatements testStatements2 DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(DynC DEPENDS dync_mutatee dync_mutator)
