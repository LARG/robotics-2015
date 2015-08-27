cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(motion)

SET(CMAKE_BUILD_TYPE "Release")

SET(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
SET(CORE_PATH ../../core)

INCLUDE_DIRECTORIES(${CORE_PATH} ${CORE_PATH}/../lib/lua/src)

SET (CORE_SRCS
  main.cpp
  ${CORE_PATH}/MotionCore.cpp 
  ${CORE_PATH}/Module.cpp
  
  ${CORE_PATH}/common/PIDController.cpp
  ${CORE_PATH}/common/NMatrix.cpp
  ${CORE_PATH}/common/WorldObject.cpp

  ${CORE_PATH}/math/MVTools.cpp
  ${CORE_PATH}/math/RotationMatrix.cpp
  ${CORE_PATH}/math/Geometry.cpp

  ${CORE_PATH}/memory/TextLogger.cpp  
  ${CORE_PATH}/memory/Logger.cpp  
  ${CORE_PATH}/memory/Lock.cpp
  ${CORE_PATH}/memory/Memory.cpp 
  ${CORE_PATH}/memory/MemoryBlock.cpp 
  ${CORE_PATH}/memory/PrivateMemory.cpp 
  ${CORE_PATH}/memory/SharedMemory.cpp

  ${CORE_PATH}/motion/KickModule.cpp
  ${CORE_PATH}/motion/MotionModule.cpp
  ${CORE_PATH}/motion/WalkEngineParameters.cpp
  ${CORE_PATH}/motion/KickEngine.cpp
  ${CORE_PATH}/motion/GetupModule.cpp
  ${CORE_PATH}/motion/WalkModule.cpp
  ${CORE_PATH}/motion/initWalkMatrices.cpp
  
  ${CORE_PATH}/sensor/SensorModule.cpp 
  ${CORE_PATH}/sensor/InertialFilter.cpp

  ${CORE_PATH}/sonar/SonarModule.cpp
  
  ${CORE_PATH}/kinematics/InverseKinematics.cpp
  ${CORE_PATH}/kinematics/ForwardKinematics.cpp 
  ${CORE_PATH}/kinematics/KinematicsModule.cpp 
  
  ${CORE_PATH}/localization/OrigKalmanFilter.cpp

  ${CORE_PATH}/../lib/alglib/alglibinternal.cpp
  ${CORE_PATH}/../lib/alglib/diffequations.cpp
  ${CORE_PATH}/../lib/alglib/linalg.cpp
  ${CORE_PATH}/../lib/alglib/statistics.cpp
  ${CORE_PATH}/../lib/alglib/alglibmisc.cpp
  ${CORE_PATH}/../lib/alglib/fasttransforms.cpp
  ${CORE_PATH}/../lib/alglib/optimization.cpp
  ${CORE_PATH}/../lib/alglib/ap.cpp
  ${CORE_PATH}/../lib/alglib/integration.cpp
  ${CORE_PATH}/../lib/alglib/solvers.cpp
  ${CORE_PATH}/../lib/alglib/dataanalysis.cpp
  ${CORE_PATH}/../lib/alglib/interpolation.cpp
  ${CORE_PATH}/../lib/alglib/specialfunctions.cpp
)

add_executable(motion ${CORE_SRCS})
target_link_libraries(motion boost_thread-mt ${BUILD_DIR}/../lua/build/liblua.a dl rt)
