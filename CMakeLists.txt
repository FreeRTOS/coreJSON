cmake_minimum_required(VERSION 3.15)

project(coreJSON LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/jsonFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library(core_json ${JSON_SOURCES})

target_include_directories(core_json PUBLIC ${JSON_INCLUDE_PUBLIC_DIRS})
