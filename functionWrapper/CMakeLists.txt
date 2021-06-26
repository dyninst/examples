project(function_wrapper LANGUAGES CXX)

add_library(wrapper MODULE wrapper.c)
target_compile_options(wrapper PRIVATE "-g")

add_executable(wrappee wrappee.c)
target_compile_options(wrappee PRIVATE "-g")

add_executable(mutator mutator.cpp)
target_include_directories(mutator PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS})
target_link_libraries(mutator dyninstAPI parseAPI ${DYNINST_Boost_LIBRARIES})

# A dummy target to allow building this example by itself
add_custom_target(functionWrapper DEPENDS mutator)