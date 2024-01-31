project(unstrip LANGUAGES CXX)

add_executable(
  unstrip
  unstrip.C
  util.C
  types.C
  semanticDescriptor.C
  database.C
  fingerprint.C
  callback.C)
target_compile_options(unstrip PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(unstrip PRIVATE Dyninst::dyninstAPI)
