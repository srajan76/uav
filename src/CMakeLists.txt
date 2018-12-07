include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


add_executable(main main.cpp instance.cpp hpath.cpp)
add_executable(generate-data generate-data.cpp instance.cpp)
add_executable(test3 test3.cpp)

target_link_libraries(main ${CPLEX_LIBRARIES} ${LEMON_LIBRARIES} ${CPP_OPTION_PARSER})
target_link_libraries(generate-data ${CPP_OPTION_PARSER})
target_link_libraries(test3 ${CPLEX_LIBRARIES})
