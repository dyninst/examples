project(maxMallocSize LANGUAGES CXX)

add_executable(maxMalloc max.cpp)
target_include_directories(maxMalloc PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS})
target_link_libraries(maxMalloc dyninstAPI ${DYNINST_Boost_LIBRARIES})

# A dummy target to allow building this example by itself
add_custom_target(maxMallocSize DEPENDS maxMalloc)
