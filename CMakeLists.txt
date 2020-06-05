cmake_minimum_required(VERSION 3.4.0)
project(examples)

# User must provide location of Dyninst cmake files either as a cache or
# environment variable
if(NOT Dyninst_DIR)
  if("$ENV{Dyninst_DIR}" STREQUAL "")
    message(
      FATAL_ERROR
        "Dyninst_DIR not found: define as a cache or environment variable")
  else()
    set(_dyninst_dir ENV{Dyninst_DIR})
  endif()
else()
  set(_dyninst_dir ${Dyninst_DIR})
  set(ENV{Dyninst_DIR} ${_dyninst_dir})
endif()

# Make sure it's an absolute path
if(NOT IS_ABSOLUTE ${_dyninst_dir})
  get_filename_component(_tmp ${_dyninst_dir} ABSOLUTE)
  set(_dyninst_dir ${_tmp})
  unset(_tmp)
endif()

set(Dyninst_DIR ${_dyninst_dir} CACHE PATH "Location of Dyninst cmake files")

# Use the Dyninst-provided CMake modules
set(CMAKE_MODULE_PATH
    "${Dyninst_DIR}"
    "${Dyninst_DIR}/Modules"
    ${CMAKE_MODULE_PATH})

# Import the Dyninst components
find_package(Dyninst REQUIRED
             COMPONENTS common
                        dyninstAPI
                        instructionAPI
                        parseAPI
                        symtabAPI
                        common)

# message(INFO "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}") set default configuration
# type
if(NOT CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE RelWithDebInfo
    CACHE
      STRING
      "Choose the build type (None, Debug, Release, RelWithDebInfo, MinSizeRel)"
    FORCE)
endif()

# Set the C and C++ language API and ABI standards
include(LanguageStandards)

# Use the same optimization settings used to build Dyninst
include(optimization)
# ----------------------------------------------------------------------------#

# Set up the individual examples
get_filename_component(EXAMPLE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
add_executable(InterestingProgram common/mutatees/InterestingProgram.cpp)
add_subdirectory(codeCoverage)
add_subdirectory(instrumentAFunction)
add_subdirectory(binaryAnalysis)
add_subdirectory(unstrip)
