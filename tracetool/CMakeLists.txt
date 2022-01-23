cmake_minimum_required(VERSION 3.13.0)
project(tracetool LANGUAGES CXX)

# Import the Dyninst components
find_package(Dyninst REQUIRED
             COMPONENTS common
                        dyninstAPI
                        instructionAPI
                        parseAPI
                        symtabAPI
                        common)

# Read the cache generated from building Dyninst
load_cache(${Dyninst_DIR}
           READ_WITH_PREFIX DYNINST_
           Boost_LIBRARIES
           Boost_INCLUDE_DIRS
           Boost_LIBRARY_DIRS
           Boost_DEFINES
           TBB_INCLUDE_DIRS)

set(WARNINGS -Wall -Wextra -Wconversion -Wsign-conversion -pedantic)

# Mutator
add_executable(trace_tool tracetool.cpp)
target_include_directories(trace_tool PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_link_libraries(trace_tool dyninstAPI ${DYNINST_Boost_LIBRARIES})

# Instrumentation library
add_library(tracelib SHARED tracelib.cpp)
target_compile_options(tracelib PRIVATE ${WARNINGS})

# Mutatee
add_library(userapp SHARED userapp.cpp)
target_compile_options(userapp PRIVATE ${WARNINGS})

# A dummy target to allow building this example by itself
add_custom_target(tracetool DEPENDS trace_tool tracelib userapp)
