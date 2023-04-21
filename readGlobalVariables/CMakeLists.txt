project(readGlobalVariables)

add_library(globals SHARED globals.cpp)

add_executable(read_global_variables reader.cpp)
target_include_directories(read_global_variables PRIVATE ${DYNINST_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${TBB_INCLUDE_DIRS})
target_link_libraries(read_global_variables PRIVATE symtabAPI common globals)