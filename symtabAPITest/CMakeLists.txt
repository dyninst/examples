project(symtabAPITest LANGUAGES CXX)

add_executable(symtabAPITest
               symtabAPITest.cpp)

target_include_directories(symtabAPITest PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_compile_definitions(symtabAPITest PRIVATE ${DYNINST_Boost_DEFINITIONS})

target_link_directories(symtabAPITest PRIVATE ${DYNINST_Boost_LIBRARY_DIRS})
target_link_libraries(symtabAPITest
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common
                      ${DYNINST_Boost_LIBRARIES}
                      )
