cmake_minimum_required(VERSION 3.22)
project(tetris_player)

set(CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_FLAGS -Wall -Wextra)

set(SOURCES_DIR ./src)
set(INCLUDE_DIR ${SOURCES_DIR}/include)
set(TARGET_SOURCES ${SOURCES_DIR}/Main.cpp)

include_directories(${INCLUDE_DIR})

set(DEFAULT_TARGET ${CMAKE_PROJECT_NAME})
set(ASAN_TARGET ${DEFAULT_TARGET}_asan)
set(MSAN_TARGET ${DEFAULT_TARGET}_msan)
set(TSAN_TARGET ${DEFAULT_TARGET}_tsan)
set(UBSAN_TARGET ${DEFAULT_TARGET}_ubsan)

add_executable(${DEFAULT_TARGET} ${TARGET_SOURCES})
add_executable(${ASAN_TARGET} ${TARGET_SOURCES})
add_executable(${MSAN_TARGET} ${TARGET_SOURCES})
add_executable(${TSAN_TARGET} ${TARGET_SOURCES})
add_executable(${UBSAN_TARGET} ${TARGET_SOURCES})

target_link_options(${ASAN_TARGET} PRIVATE
        -fsanitize=address -fno-omit-frame-pointer)
target_link_options(${MSAN_TARGET} PRIVATE -fsanitize=memory)
target_link_options(${TSAN_TARGET} PRIVATE -fsanitize=thread)
target_link_options(${UBSAN_TARGET} PRIVATE -fsanitize=undefined)
