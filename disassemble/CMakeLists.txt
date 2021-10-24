project(unstrip LANGUAGES CXX)

add_executable(disassemble
               disassemble.cpp)

target_include_directories(disassemble PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_compile_definitions(disassemble PRIVATE ${DYNINST_Boost_DEFINITIONS})

target_link_directories(disassemble PRIVATE ${DYNINST_Boost_LIBRARY_DIRS})
target_link_libraries(disassemble
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common
                      ${DYNINST_Boost_LIBRARIES}
                      )
