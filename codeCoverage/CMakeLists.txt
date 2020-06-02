cmake_minimum_required(VERSION 3.4.0)
project(codeCoverage)

add_executable(codeCoverage codeCoverage.C)
add_dependencies(codeCoverage dyninstAPI patchAPI parseAPI symtabAPI instructionAPI pcontrol common stackwalk dynDwarf dynElf)
target_link_libraries(codeCoverage dyninstAPI patchAPI parseAPI symtabAPI instructionAPI pcontrol common stackwalk dynDwarf dynElf ${Boost_LIBRARIES} ${ElfUtils_LIBRARIES})
