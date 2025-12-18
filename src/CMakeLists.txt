# Collect source files
file(GLOB_RECURSE SRC_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include direcotries (for headers)
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${PROJECT_SOURCE_DIR}/vendor/nanosvg/src # header-only lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Debug: show all sources
message(STATUS "nanosvg lib dir = ${PROJECT_SOURCE_DIR}/vendor/nanosvg/src")

message(STATUS "src files:")
foreach(f IN LISTS SRC_FILES)
    message(STATUS "  ${f}")
endforeach()