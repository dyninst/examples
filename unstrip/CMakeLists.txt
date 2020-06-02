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

target_include_directories(unstrip PRIVATE ${DYNINST_INCLUDE_DIR})

target_link_libraries(unstrip
                      dyninstAPI
                      instructionAPI
                      parseAPI
                      symtabAPI
                      common)
