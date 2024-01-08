project(patchAPI LANGUAGES CXX)

add_executable(CFGTraversal CFGTraversal.cpp)
target_compile_options(CFGTraversal PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(CFGTraversal PRIVATE Dyninst::patchAPI)

add_executable(findingPoints findingPoints.cpp)
target_compile_options(findingPoints PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(findingPoints PRIVATE Dyninst::patchAPI)

add_executable(nopPatching nopPatching.cpp)
target_compile_options(nopPatching PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(nopPatching PRIVATE Dyninst::patchAPI)

add_library(addressSpace SHARED addressSpace.cpp)
target_compile_options(addressSpace PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(addressSpace PRIVATE Dyninst::patchAPI)

add_executable(CFGMaker CFGMaker.cpp)
target_compile_options(CFGMaker PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(CFGMaker PRIVATE Dyninst::patchAPI)

add_executable(PointMaker PointMaker.cpp)
target_compile_options(PointMaker PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(PointMaker PRIVATE Dyninst::patchAPI)

add_executable(PatchModifier PatchModifier.cpp)
target_compile_options(PatchModifier PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(PatchModifier PRIVATE Dyninst::patchAPI)
