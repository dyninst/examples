project(compareLLVM LANGUAGES CXX)

add_executable(compareLLVM disassemble.cpp)
target_compile_options(compareLLVM PRIVATE ${EXAMPLES_WARNING_FLAGS})
target_link_libraries(compareLLVM Dyninst::dyninstAPI)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/compareLLVM/parser.py
  COMMAND ${CMAKE_COMMAND} -E create_symlink
          "${CMAKE_SOURCE_DIR}/compareLLVM/parser.py"
          "${CMAKE_BINARY_DIR}/compareLLVM/parser.py"
  COMMENT "Creating Symlink for compareLLVM/parser.py"
)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/compareLLVM/run.sh
  COMMAND ${CMAKE_COMMAND} -E create_symlink
          "${CMAKE_SOURCE_DIR}/compareLLVM/run.sh"
          "${CMAKE_BINARY_DIR}/compareLLVM/run.sh"
  COMMENT "Creating Symlink for compareLLVM/run.sh"
)

#add_custom_target(create_my_link ALL DEPENDS ${CMAKE_BINARY_DIR}/compareLLVM/parser.py)

add_custom_target(create_my_link ALL DEPENDS ${CMAKE_BINARY_DIR}/compareLLVM/parser.py ${CMAKE_BINARY_DIR}/compareLLVM/run.sh)
