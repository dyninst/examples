project(memoryAccessCounter LANGUAGES CXX)

add_executable(counter counter.cpp)
target_include_directories(counter PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_link_libraries(counter dyninstAPI instructionAPI ${DYNINST_Boost_LIBRARIES})

install(TARGETS InterestingProgram  RUNTIME DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(memoryAccessCounter DEPENDS counter)
