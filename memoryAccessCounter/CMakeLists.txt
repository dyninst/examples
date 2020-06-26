project(memoryAccessCounter LANGUAGES CXX)

add_executable(counter counter.cpp)
target_include_directories(counter PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(counter dyninstAPI instructionAPI ${DYNINST_Boost_LIBRARIES})

# A dummy target to allow building this example by itself
add_custom_target(memoryAccessCounter DEPENDS counter)
