cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

include("${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.cmake")
use(NAOQI-PLUGINS-TOOLS)

create_module( naointerface )

SET(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

INCLUDE_DIRECTORIES(../../core)

SET ( NAOINTERFACE_SRCS
  src/naointerfacemain.cpp
  src/naointerface.cpp
  src/dcmwrapper.cpp
  src/almotionwrapper.cpp
  src/WhistleDetectionModule.cpp

  ../../core/memory/SharedMemory.cpp
  ../../core/memory/PrivateMemory.cpp
  ../../core/memory/Memory.cpp
  ../../core/memory/MemoryBlock.cpp
  ../../core/memory/Lock.cpp
  ../../core/math/Geometry.cpp
  ../../core/math/RotationMatrix.cpp
  ../../core/motion/WalkEngineParameters.cpp
)

configure_src_module( naointerface ${NAOINTERFACE_SRCS} )

TARGET_LINK_LIBRARIES(naointerface)

use_lib(naointerface ALCOMMON ALMEMORY_FAST_ACCESS ${LIBFFT})
 
