cmake_minimum_required(VERSION 3.13.0)
project(examples)

find_package(Dyninst 12.3.0 REQUIRED COMPONENTS dyninstAPI dynC_API)

add_executable(InterestingProgram common/mutatees/InterestingProgram.cpp)

set(EXAMPLES_WARNING_FLAGS -Wall -Wextra)

add_subdirectory(CFGraph)
add_subdirectory(codeCoverage)
add_subdirectory(disassemble)
add_subdirectory(DynC)
add_subdirectory(instrumentAFunction)
add_subdirectory(instrumentMemoryAccess)
add_subdirectory(insertSnippet)
add_subdirectory(interceptOutput)
add_subdirectory(maxMallocSize)
add_subdirectory(memoryAccessCounter)
add_subdirectory(tracetool)
add_subdirectory(readGlobalVariables)
add_subdirectory(unstrip)
add_subdirectory(unusedArgs)
