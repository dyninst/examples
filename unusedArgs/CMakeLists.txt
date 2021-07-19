project(unusedArgs LANGUAGES CXX)

add_executable(unusedArgs
               unusedArgs.C)

target_include_directories(unusedArgs PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS})
target_compile_definitions(unusedArgs PRIVATE ${DYNINST_Boost_DEFINITIONS})

target_link_directories(unusedArgs PRIVATE ${DYNINST_Boost_LIBRARY_DIRS})
target_link_libraries(unusedArgs
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common
                      ${DYNINST_Boost_LIBRARIES}
                      )
