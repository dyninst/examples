project(insertSnippet LANGUAGES CXX)

add_executable(noop_snippet noop_snippet.cpp)
target_compile_options(noop_snippet PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(noop_snippet PRIVATE Dyninst::dyninstAPI)
