project(interceptOutput LANGUAGES CXX)

add_executable(retee retee.cpp)
target_include_directories(retee PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_link_libraries(retee dyninstAPI ${DYNINST_Boost_LIBRARIES})

# A dummy target to allow building this example by itself
add_custom_target(interceptOutput DEPENDS retee)
