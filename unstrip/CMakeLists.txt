cmake_minimum_required(VERSION 3.4.0)
project(unstrip LANGUAGES CXX)

add_executable(unstrip
               unstrip.C
               util.C
               types.C
               semanticDescriptor.C
               database.C
               fingerprint.C
               callback.C)

target_include_directories(unstrip PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS})
target_compile_definitions(unstrip PRIVATE ${DYNINST_Boost_DEFINITIONS})

target_link_directories(unstrip PRIVATE ${DYNINST_Boost_LIBRARY_DIRS})
target_link_libraries(unstrip
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common
                      ${DYNINST_Boost_LIBRARIES}
                      )
