add_subdirectory(SDL)

# CHACHE BOOL makes it a cach variable, so it can be overridden by the command line if needed
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries by default")

add_library(vendor INTERFACE)
target_link_directories(vendor INTERFACE SDL3::SDL3)