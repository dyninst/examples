project(stackwalker LANGUAGES CXX)

add_executable(this_thread this_thread.cpp)
target_compile_options(this_thread PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(this_thread Dyninst::stackwalk)

add_executable(determineWalkerParty determineWalkerParty.cpp)
target_compile_options(determineWalkerParty PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(determineWalkerParty Dyninst::stackwalk)

add_library(simple3rdPartyWalk SHARED simple3rdPartyWalk.cpp)
target_compile_options(simple3rdPartyWalk PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(simple3rdPartyWalk Dyninst::stackwalk)

add_library(complex3rdPartyWalk SHARED complex3rdPartyWalk.cpp)
target_compile_options(complex3rdPartyWalk PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(complex3rdPartyWalk Dyninst::stackwalk)
