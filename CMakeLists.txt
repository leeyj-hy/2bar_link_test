cmake_minimum_required(VERSION 2.8.12)
project (EPOS_2BAR)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PJT_SRC_FILES Epos4_2bar_test.cpp)
add_executable(${PROJECT_NAME} ${PJT_SRC_FILES})

target_link_libraries(${PROJECT_NAME} -lEposCmd)

