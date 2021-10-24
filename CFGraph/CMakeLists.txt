project(CFGraph LANGUAGES CXX)

add_executable(CFGraph
               CFGraph.cpp)

target_include_directories(CFGraph PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_compile_definitions(CFGraph PRIVATE ${DYNINST_Boost_DEFINITIONS})

target_link_directories(CFGraph PRIVATE ${DYNINST_Boost_LIBRARY_DIRS})
target_link_libraries(CFGraph
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common
                      ${DYNINST_Boost_LIBRARIES}
                      )
