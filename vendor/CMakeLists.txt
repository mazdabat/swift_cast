add_subdirectory(SDL)
add_subdirectory(nanosvg)

# CHACHE BOOL makes it a cache variable, so it can be overridden by the command line if needed
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries by default")

# Add libraries one by one here

# SDL3::SDL3 is an imported CMake target provided by SDL3's CMake configuration

#[[ Explanation of SDL3::SDL3
<namespace>::<target> means the library, with all its settings attached.

CMake automatically applies:
- The correct library (libSDL3.so / .a)
- Include direcotries (#include <SDL3/SDL.h>)
- Required compile definitions
- Platform-specific linker flags
- Transitive dependencies

You don't need to manually write:
include_directories(...)
link_directories(...)
target_compile_definitions(...)

The namespace and target come from:
1. find_package(SDL3 REQUIRE), or
2. add_library(SDL3::SDL3 IMPORTED)

So SDL3::SDL3 already exists before you use it

]]#

# Lib - SDL3
add_library(SDL3 INTERFACE ${CMAKE_SOURCE_DIR}/SDL)
target_link_directories(vendor INTERFACE SDL3::SDL3)

