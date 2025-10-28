cmake_minimum_required(VERSION 3.25)
project(unstdlib VERSION 1.0.0 DESCRIPTION "Pure unstandard C library.")

set(CMAKE_C_STANDARD 11)

#include_directories(src)
#include(CMakePrintHelpers)
#cmake_print_variables(library_files)

# ------------------------ [ global variables ] ------------------------
set(INSTALLATION_DIRECTORY_UNSTDLIB_STATIC /usr/local/lib)
set(INSTALLATION_DIRECTORY_UNSTDLIB_SHARED /usr/local/lib)

# ------------------------ [ unstdlib files ] ------------------------
set(locals_unstdlib
        src/unstdlib.h
        src/_unstdcommon.h
        src/_unstdcommon.c
)

# - references
file(GLOB sources_unstdlib
        src/unstd*.c
)

file(GLOB headers_unstdlib
        src/unstd*.h
)

set(sources_and_headers_unstdlib
        ${sources_unstdlib}
        ${headers_unstdlib}
)

set(all_unstdlib
        ${locals_unstdlib}
        ${sources_and_headers_unstdlib}
)

file(GLOB library_files ${sources_and_headers_unstdlib})
file(GLOB test_source_files test/test_headers/*/*.c)

# ------------------------ [ unstdlib shared library [all] ] ------------------------
add_library(unstdlib_shared SHARED
        ${library_files}
)

set_target_properties(unstdlib_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(unstdlib_shared PROPERTIES PUBLIC_HEADER src/unstdlib.h)
set_target_properties(unstdlib_shared PROPERTIES OUTPUT_NAME unstdlib)

include(GNUInstallDirs)
install(TARGETS unstdlib_shared
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #        ARCHIVE DESTINATION ${INSTALLATION_DIRECTORY_UNSTDLIB_SHARED}
        #        LIBRARY DESTINATION ${INSTALLATION_DIRECTORY_UNSTDLIB_SHARED}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
configure_file(unstdlib.pc.in unstdlib.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/unstdlib.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

# ------------------------ [ unstdlib static library [all] ] ------------------------
add_library(unstdlib_static STATIC
        ${library_files}
)

set_target_properties(unstdlib_static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(unstdlib_static PROPERTIES PUBLIC_HEADER src/unstdlib.h)
set_target_properties(unstdlib_static PROPERTIES OUTPUT_NAME unstdlib)

include(GNUInstallDirs)
install(TARGETS unstdlib_static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #        ARCHIVE DESTINATION ${INSTALLATION_DIRECTORY_UNSTDLIB_STATIC}
        #        LIBRARY DESTINATION ${INSTALLATION_DIRECTORY_UNSTDLIB_STATIC}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
configure_file(unstdlib.pc.in unstdlib.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/unstdlib.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

# ------------------------ [ unstdlib testing ] ------------------------
# - locals
set(locals_test
        test/_test_common.h
)

#add_library(. ${library_files})
foreach (test_source_file ${test_source_files})
    get_filename_component(test_source_file_name ${test_source_file} NAME_WE)
    add_executable(${test_source_file_name} ${test_source_file})
    #    target_link_libraries(${test_source_file_name} .)
    target_link_libraries(${test_source_file_name} unstdlib_static)
endforeach (test_source_file ${test_source_files})

# ------------------------ [ unstdlib testing [all] ] ------------------------
set(test_unstdlib
        test/test_unstdlib.h
        test/test_unstdlib.c
)

add_executable(test_all
        ${locals_test}
        ${test_unstdlib}
        ${all_unstdlib}
)
