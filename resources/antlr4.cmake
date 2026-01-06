# Taken from https://github.com/antlr/antlr4/blob/master/runtime/Cpp/cmake/README.md

set(ANTLR4_TAG "4.13.2")
set(ANTLR4_ZIP_REPOSITORY https://github.com/antlr/antlr4/archive/refs/tags/${ANTLR4_TAG}.zip)

set(ANTLR4_WITH_STATIC_CRT OFF) # using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_BUILD_SHARED ON) # built the shared version
set(ANTLR4_BUILD_CPP_TESTS OFF) # avoid creating dist folder

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources/cmake)

# add external build for antlrcpp
include(ExternalAntlr4Cpp)

# set variable pointing to the antlr tool that supports C++
set(ANTLR_EXECUTABLE "${CMAKE_BINARY_DIR}/resources/antlr-${ANTLR4_TAG}-complete.jar")
if(NOT EXISTS "${ANTLR_EXECUTABLE}")
    message(STATUS "Downloading ANTLR ${ANTLR4_TAG}")
    file(DOWNLOAD "https://www.antlr.org/download/antlr-${ANTLR4_TAG}-complete.jar" "${ANTLR_EXECUTABLE}")
    message(STATUS "Finished downloading ANTLR")
endif()

# Find ANTLR4
find_package(ANTLR REQUIRED)

add_library(antlr4 SHARED IMPORTED)
add_dependencies(antlr4 antlr4_shared)

set_target_properties(
        antlr4
        PROPERTIES
        IMPORTED_LOCATION ${ANTLR4_RUNTIME_LIBRARIES}
        INTERFACE_INCLUDE_DIRECTORIES ${ANTLR4_INCLUDE_DIRS}
)
