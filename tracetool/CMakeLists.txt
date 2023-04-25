project(tracetool LANGUAGES CXX)

# Mutator
add_executable(trace_tool tracetool.cpp)
target_compile_options(trace_tool PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(trace_tool PRIVATE Dyninst::dyninstAPI)

# Instrumentation library
add_library(tracelib SHARED tracelib.cpp)
target_compile_options(tracelib PRIVATE ${EXAMPLES_WARNING_FLAGS})

# Mutatee
add_library(userapp SHARED userapp.cpp)
target_compile_options(userapp PRIVATE ${EXAMPLES_WARNING_FLAGS})
