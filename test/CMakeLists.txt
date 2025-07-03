# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/Unity/src)

# Add the Unity source files
set(UNITY_SRC ${PROJECT_SOURCE_DIR}/Unity/src/unity.c)

# Add the test executable
add_executable(CsvTest test_csv.c ${UNITY_SRC})
target_link_libraries(CsvTest csv)

# Add the test executable for person
add_executable(PersonTest test_person.c ${UNITY_SRC})
target_link_libraries(PersonTest person)

# Add the test executable for coordinate_node
add_executable(CoordinateNodeTest test_coordinate_node.c ${UNITY_SRC})
target_link_libraries(CoordinateNodeTest coordinate_node)

# Add the test executable for temporal_node
add_executable(TemporalNodeTest test_temporal_node.c ${UNITY_SRC})
target_link_libraries(TemporalNodeTest temporal_node coordinate_node date)

# Add the test executable for api
add_executable(ApiTest test_api.c ${UNITY_SRC})
target_link_libraries(ApiTest api csv person temporal_node coordinate_node date)

# Copy the CSV file to the build directory
file(COPY ${PROJECT_SOURCE_DIR}/test/example_1.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/test/example_2.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add tests
add_test(NAME test_csv COMMAND CsvTest)
add_test(NAME test_person COMMAND PersonTest)
add_test(NAME test_coordinate_node COMMAND CoordinateNodeTest)
add_test(NAME test_temporal_node COMMAND TemporalNodeTest)
add_test(NAME test_api COMMAND ApiTest)
