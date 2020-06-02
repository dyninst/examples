cmake_minimum_required(VERSION 3.4.0)
project(examples)

add_directory(codeCoverage)
add_directory(unstrip)

if (USE_OpenMP)
set_target_properties (unstrip PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
set_target_properties (codeCoverage PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
set_target_properties (cfg_to_dot PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
endif()


install (TARGETS cfg_to_dot unstrip codeCoverage Inst
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
)

install (
        FILES unstrip.dir/ddb.db unstrip.dir/params.db unstrip.dir/unistd.db DESTINATION ${INSTALL_BIN_DIR}
)
