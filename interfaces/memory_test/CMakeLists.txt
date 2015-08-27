cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(memory_test)

#SET( CMAKE_VERBOSE_MAKEFILE on)

SET(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
SET(CORE_PATH ../../core)

INCLUDE_DIRECTORIES(${CORE_PATH} ${CORE_PATH}/../lib/lua/src)

SET (CORE_SRCS
  main.cpp
  ${CORE_PATH}/memory/MemoryBlock.cpp 
  ${CORE_PATH}/memory/SharedMemory.cpp 
)

add_executable(memory_test ${CORE_SRCS})
target_link_libraries(memory_test boost_thread-mt)
