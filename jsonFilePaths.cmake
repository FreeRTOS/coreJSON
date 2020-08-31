# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# Root directory of the JSON library.
set( MODULE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "JSON source root." )

# JSON library source files.
set( JSON_SOURCES
     ${MODULE_ROOT_DIR}/source/json.c )

# JSON library Public Include directories.
set( JSON_INCLUDE_PUBLIC_DIRS
     ${MODULE_ROOT_DIR}/include )
