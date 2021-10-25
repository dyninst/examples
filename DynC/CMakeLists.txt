project(DynC)

add_executable(dync_mutatee mutatee.cpp)

add_executable(dync_mutator mutator.cpp)
target_include_directories(dync_mutator PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_link_libraries(dync_mutator PRIVATE dynC_API dyninstAPI)

install(FILES testStatements testStatements2 DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(DynC DEPENDS dync_mutatee dync_mutator)
