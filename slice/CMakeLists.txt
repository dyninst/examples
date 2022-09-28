project(slice LANGUAGES CXX)

add_executable(slice
               slice.C
)

target_include_directories(slice PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
target_compile_definitions(slice PRIVATE ${DYNINST_Boost_DEFINITIONS})

target_link_directories(slice PRIVATE ${DYNINST_Boost_LIBRARY_DIRS})
target_link_libraries(slice
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common
                      ${DYNINST_Boost_LIBRARIES}
                      )
