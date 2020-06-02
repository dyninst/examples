cmake_minimum_required(VERSION 3.4.0)
project(unstrip)

add_executable(unstrip unstrip.C
                       util.C
                       types.C
                       semanticDescriptor.C
                       database.C
                       fingerprint.C
                       callback.C)
add_dependencies(unstrip parseAPI symtabAPI instructionAPI common)
target_link_libraries(unstrip parseAPI symtabAPI instructionAPI common dynDwarf dynElf ${Boost_LIBRARIES} ${ElfUtils_LIBRARIES})