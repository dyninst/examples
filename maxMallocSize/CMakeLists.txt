project(maxMallocSize LANGUAGES CXX)

add_executable(maxMalloc max.cpp)
target_include_directories(maxMalloc PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(maxMalloc dyninstAPI ${Boost_SYSTEM_LIBRARY})

# A dummy target to allow building this example by itself
add_custom_target(maxMallocSize DEPENDS maxMalloc)
