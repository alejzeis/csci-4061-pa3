cmake_minimum_required(VERSION 3.14)

project(PA3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/image_rotation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/utils.o
)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(PA3 ${SOURCE_FILES})
target_include_directories(PA3 PRIVATE ${INCLUDE_DIR})
target_link_libraries(PA3 m)
