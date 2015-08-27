cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(core)

#SET( CMAKE_VERBOSE_MAKEFILE on)

SET(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
SET(CORE_PATH ../../core)

INCLUDE_DIRECTORIES(${CORE_PATH} /usr/local/include/simspark/ /usr/include/simspark/)

SET (SIM_SRCS
  main.cpp  
  Behavior.cpp
  RobotBehavior.cpp
  Parser.cpp
  cdecode.c
  cencode.c
  ${CORE_PATH}/common/WorldObject.cpp
  ${CORE_PATH}/motion/WalkEngineParameters.cpp
  ${CORE_PATH}/memory/Lock.cpp
  ${CORE_PATH}/memory/Memory.cpp
  ${CORE_PATH}/memory/MemoryBlock.cpp
  ${CORE_PATH}/memory/PrivateMemory.cpp
  ${CORE_PATH}/memory/SharedMemory.cpp
  ${CORE_PATH}/math/Geometry.cpp
)

link_directories(/usr/lib/simspark /usr/lib/simspark/rcssnet /usr/local/lib/simspark /usr/local/lib/simspark/rcssnet)  # For some reason this has to be before the add_execuatble

add_executable(sim ${SIM_SRCS})

target_link_libraries(sim boost_thread-mt rcssnet3D dl rt)
