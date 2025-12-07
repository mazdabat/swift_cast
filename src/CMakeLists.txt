# find_package(SDL3 REQUIRED)

include_directories(${SDL3_INCLUDE_DIRS})

add_executable(${PROJECT_NAME})

# Add source code
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

# Link SDL3 to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)