cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(vision)

#SET(CMAKE_VERBOSE_MAKEFILE on)
SET(CMAKE_BUILD_TYPE Release)

SET(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
SET(CORE_PATH ../../core)

INCLUDE_DIRECTORIES(${CORE_PATH} ${CORE_PATH}/../lib/lua/src)

SET (CORE_SRCS
  main.cpp
  ${CORE_PATH}/VisionCore.cpp 
  ${CORE_PATH}/core_wrap.cpp
  ${CORE_PATH}/Module.cpp
  ${CORE_PATH}/common/StateMachine.cpp 
  ${CORE_PATH}/common/WorldObject.cpp 
  ${CORE_PATH}/common/NMatrix.cpp 
  ${CORE_PATH}/communications/CommunicationModule.cpp 
  #${CORE_PATH}/communications/ThreadedUDPSocket.cpp 
  ${CORE_PATH}/communications/UDPWrapper.cpp 
  ${CORE_PATH}/kinematics/InverseKinematics.cpp
  ${CORE_PATH}/lua/LuaModule.cpp 
  ${CORE_PATH}/lua/LuaInterp.cpp 
  ${CORE_PATH}/math/Geometry.cpp
  ${CORE_PATH}/math/RotationMatrix.cpp
  ${CORE_PATH}/math/MVTools.cpp
  ${CORE_PATH}/memory/Lock.cpp  
  ${CORE_PATH}/memory/Logger.cpp  
  ${CORE_PATH}/memory/Memory.cpp 
  ${CORE_PATH}/memory/MemoryBlock.cpp 
  ${CORE_PATH}/memory/PrivateMemory.cpp 
  ${CORE_PATH}/memory/SharedMemory.cpp
  ${CORE_PATH}/memory/TextLogger.cpp
  ${CORE_PATH}/lights/LEDModule.cpp
  ${CORE_PATH}/localization/BallKF.cpp
  ${CORE_PATH}/localization/OrigKalmanFilter.cpp
  ${CORE_PATH}/localization/PerfectLocalizationModule.cpp
  ${CORE_PATH}/localization/UKFModule.cpp
  ${CORE_PATH}/localization/UKF.cpp
  ${CORE_PATH}/motion/WalkEngineParameters.cpp
  ${CORE_PATH}/opponents/OppModule.cpp
  ${CORE_PATH}/opponents/UKF4.cpp
  ${CORE_PATH}/sensor/ButtonModule.cpp
  ${CORE_PATH}/vision/VisionModule.cpp 
  ${CORE_PATH}/vision/RobotDetection.cpp 
  ${CORE_PATH}/vision/LineDetection.cpp 
  ${CORE_PATH}/vision/GoalDetection.cpp 
  ${CORE_PATH}/vision/BallDetection.cpp
  ${CORE_PATH}/vision2/TransformationProvider.cpp
  ${CORE_PATH}/behavior/BehaviorModule.cpp


  ${CORE_PATH}/imageCapture/ImageCapture.cpp
  ${CORE_PATH}/imageCapture/NaoCamera.cpp
)

add_executable(vision ${CORE_SRCS})
target_link_libraries(vision boost_thread-mt boost_system-mt ${BUILD_DIR}/../lua/build/liblua.a dl ${UT_LIB_DIR}/libz.a rt)
