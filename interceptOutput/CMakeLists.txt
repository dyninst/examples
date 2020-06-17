project(interceptOutput LANGUAGES CXX)

add_executable(retee retee.cpp)
target_include_directories(retee PRIVATE ${DYNINST_INCLUDE_DIR})
target_link_libraries(retee dyninstAPI ${Boost_SYSTEM_LIBRARY})

# A dummy target to allow building this example by itself
add_custom_target(interceptOutput DEPENDS retee)
