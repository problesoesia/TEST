cmake_minimum_required(VERSION 3.10)
project(HelloWorld)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/Unity/src)

# Add the library
add_library(csv src/csv.c)
add_library(date src/date.c)
add_library(person src/person.c)
add_library(api src/api.c)
add_library(coordinate_node src/coordinate_node.c)
add_library(temporal_node src/temporal_node.c)

# Add the main executable
add_executable(CDevOps src/main.c src/api.c src/temporal_node.c)
# target_link_libraries(CDevOps csv)
# target_link_libraries(CDevOps date)
# target_link_libraries(CDevOps person)
# target_link_libraries(CDevOps coordinate_node)
# target_link_libraries(CDevOps temporal_node)
# target_link_libraries(CDevOps api)

target_link_libraries(CDevOps csv date person coordinate_node temporal_node api)


# Enable testing
enable_testing()

# Add the test directory
add_subdirectory(test)
