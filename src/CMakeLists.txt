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
)

#[[
# Add source code
target_sources(${PROJECT_NAME} PRIVATE 
    # Best Practice (manual listing here)
)
]]#

# Link SDL3 to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Debug: show all sources
message(STATUS, "Current Source Dir = ${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "src files:")
foreach(f IN LISTS SRC_FILES)
    message(STATUS "  ${f}")
endforeach()