project(instrumentMemoryAccess LANGUAGES CXX)

add_executable(instrumenting_memory_access instrumenting_memory_access.cpp)
target_include_directories(instrumenting_memory_access PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS})
target_link_libraries(instrumenting_memory_access dyninstAPI instructionAPI ${DYNINST_Boost_LIBRARIES})

install(TARGETS InterestingProgram  RUNTIME DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(instrumentMemoryAccess DEPENDS instrumenting_memory_access)
