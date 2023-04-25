project(codeCoverage LANGUAGES CXX)

# The main code coverage tool
add_executable(code_coverage code_coverage.C)
target_compile_options(code_coverage PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(code_coverage PRIVATE Dyninst::dyninstAPI)

# An example instrumentation library
add_library(Inst SHARED Inst.C)
target_compile_options(Inst PRIVATE ${EXAMPLES_WARNING_FLAGS})

# An example external library used by testcc
add_library(external SHARED external.C)
target_compile_options(external PRIVATE ${EXAMPLES_WARNING_FLAGS})

# An example binary to be manipulated by codeCoverage
add_executable(testcc testcc.C)
target_compile_options(testcc PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(testcc PRIVATE Inst external)

# Some builds of gcc have PIE enabled by default. Dyninst doesn't handle them
# correctly, so disable this
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(testcc PRIVATE "-fno-pie")
  target_link_options(testcc PRIVATE "-no-pie")
endif()
