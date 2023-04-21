project(maxMallocSize LANGUAGES CXX)

add_executable(maxMalloc max.cpp)
target_compile_options(maxMalloc PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(maxMalloc PRIVATE Dyninst::dyninstAPI)

# A dummy target to allow building this example by itself
add_custom_target(maxMallocSize DEPENDS maxMalloc)
