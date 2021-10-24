project(instrumentAFunction LANGUAGES CXX)

add_executable(instrumenting_a_function instrumenting_a_function.cpp)
target_include_directories(instrumenting_a_function PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_link_libraries(instrumenting_a_function dyninstAPI ${DYNINST_Boost_LIBRARIES})

install(TARGETS InterestingProgram  RUNTIME DESTINATION ${PROJECT_BINARY_DIR})

# A dummy target to allow building this example by itself
add_custom_target(instrumentAFunction DEPENDS instrumenting_a_function)