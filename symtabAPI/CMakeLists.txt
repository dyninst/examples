project(symtabAPI LANGUAGES CXX)

add_executable(printSymbols printSymbols.cpp)
target_compile_options(printSymbols PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(printSymbols PRIVATE Dyninst::symtabAPI)

add_executable(addSymbol addSymbol.cpp)
target_compile_options(addSymbol PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(addSymbol PRIVATE Dyninst::symtabAPI)

add_executable(addType addType.cpp)
target_compile_options(addType PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(addType PRIVATE Dyninst::symtabAPI)

add_executable(printLineInfo printLineInfo.cpp)
target_compile_options(printLineInfo PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(printLineInfo PRIVATE Dyninst::symtabAPI)

add_executable(printLocalVars printLocalVars.cpp)
target_compile_options(printLocalVars PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(printLocalVars PRIVATE Dyninst::symtabAPI)
