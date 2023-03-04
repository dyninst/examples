project(insertSnippet LANGUAGES CXX)

add_executable(noop_snippet noop_snippet.cpp)
target_include_directories(noop_snippet PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_link_libraries(noop_snippet dyninstAPI common instructionAPI patchAPI ${DYNINST_Boost_LIBRARIES})
