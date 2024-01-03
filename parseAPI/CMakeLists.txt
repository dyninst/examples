project(parseAPI LANGUAGES CXX)

add_library(loopAnalysis SHARED loopAnalysis.cpp)
target_compile_options(loopAnalysis PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(loopAnalysis PRIVATE Dyninst::parseAPI)

add_library(edgePredicate SHARED edgePredicate.cpp)
target_compile_options(edgePredicate PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(edgePredicate PRIVATE Dyninst::parseAPI)
