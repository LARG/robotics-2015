
############################ PROJECT SOURCES FILES
# Add source files needed to compile this project

SET(RSWALK_SRCS
   
   # Vision
   perception/vision/RSCamera.cpp
   perception/vision/VisionDefs.cpp

   # Kinematics
   perception/kinematics/Kinematics.cpp
   perception/kinematics/Pose.cpp
   perception/kinematics/Parameters.cpp
   perception/kinematics/CKF.cpp

   # Types
   types/XYZ_Coord.hpp
   types/PostInfo.cpp

   # Misc
   #utils/Logger.cpp
   thread/Thread.cpp

   # YAML
   GyroConfig.cpp

   # Motion
   ActionGenerator.cpp
   BodyModel.cpp
   ClippedGenerator.cpp
   DeadGenerator.cpp
   DistributedGenerator.cpp
   Walk2014Generator.cpp
   WalkCycle.cpp
   PendulumModel.cpp
   WalkEnginePreProcessor.cpp
   RSWalkModule2014.cpp
   HeadGenerator.cpp
   NullGenerator.cpp
   RefPickupGenerator.cpp
   StandGenerator.cpp
   )

############################ CHECK LIBRARY / EXECUTABLE OPTION

ADD_LIBRARY(rswalk2014 STATIC ${RSWALK_SRCS} )
#ADD_LIBRARY(soccer-static STATIC ${RSWALK_SRCS})
ADD_LIBRARY(rssoccer STATIC version.cpp)

find_package(PythonLibs 2 REQUIRED)

############################ INCLUDE DIRECTORY
# Define include directories here
#set_source_files_properties(
#   perception/behaviour/python/PythonSkill.cpp
#   perception/behaviour/BehaviourAdapter.cpp
#   perception/behaviour/python/RobotModule.cpp
#   perception/behaviour/python/RegisterConverters.cpp
#   PROPERTIES COMPILE_FLAGS "-I${PYTHON_INCLUDE_DIR}")
#if(CMAKE_TOOLCHAIN_FILE)#this is temporary while the toolchain is broken
#   set_source_files_properties(
#      perception/vision/Vocab.cpp
#      perception/vision/Tfidf.cpp
#      PROPERTIES COMPILE_FLAGS "-Wno-sign-compare")
#endif(CMAKE_TOOLCHAIN_FILE)#this is temporary while the toolchain is broken

SET_TARGET_PROPERTIES(rswalk2014 PROPERTIES OUTPUT_NAME "rswalk2014")
SET_TARGET_PROPERTIES(rswalk2014 PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(rssoccer PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(rswalk2014 PROPERTIES CLEAN_DIRECT_OUTPUT 1)

TARGET_LINK_LIBRARIES(rssoccer rswalk2014)

ADD_CUSTOM_COMMAND ( OUTPUT version.cpp
   COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/genversion.pl > version.cpp
)

SET ( SWIG_DEPENDENCIES
#   perception/behaviour/python/robot.i
#   perception/behaviour/python/eigen.i
   utils/body.hpp
   utils/boostSerializationVariablesMap.hpp
   utils/SPLDefs.hpp
   utils/speech.hpp
   perception/kinematics/Parameters.hpp
#   perception/vision/RobotRegion.hpp
   perception/vision/WhichCamera.hpp
   perception/kinematics/Pose.hpp
#   gamecontroller/RoboCupGameControlData.hpp
#   types/BehaviourRequest.hpp
   types/Point.hpp
#   types/PPoint.hpp
   types/BBox.hpp
   types/ActionCommand.hpp
#   types/ButtonPresses.hpp
   types/Odometry.hpp
   types/JointValues.hpp
   types/SensorValues.hpp
   types/RRCoord.hpp
   types/AbsCoord.hpp
#   types/BroadcastData.hpp
#   types/FootInfo.hpp
#   types/BallInfo.hpp
   types/PostInfo.hpp
#   types/RobotInfo.hpp
#   types/FieldEdgeInfo.hpp
#   types/FieldFeatureInfo.hpp
#   blackboard/Blackboard.hpp
)

ADD_DEFINITIONS( -DEIGEN_DONT_ALIGN )

#ADD_CUSTOM_COMMAND ( OUTPUT RobotModule.cpp
#   COMMAND swig2.0 -Wextra -w509 -python -c++ -I${CMAKE_CURRENT_SOURCE_DIR} -o RobotModule.cpp ${CMAKE_CURRENT_SOURCE_DIR}/perception/behaviour/python/robot.i
#        && patch robot.py ${CMAKE_CURRENT_SOURCE_DIR}/perception/behaviour/python/robot.py.patch
#        && patch RobotModule.cpp ${CMAKE_CURRENT_SOURCE_DIR}/perception/behaviour/python/RobotModule.cpp.patch
#        && mv robot.py ${CMAKE_CURRENT_SOURCE_DIR}/../image/home/nao/data/behaviours/robot.py
#   DEPENDS ${SWIG_DEPENDENCIES}
#)

#SET_SOURCE_FILES_PROPERTIES( RobotModule.cpp PROPERTIES GENERATED TRUE )
SET_SOURCE_FILES_PROPERTIES( version.cpp PROPERTIES GENERATED TRUE )
SET_SOURCE_FILES_PROPERTIES( log.cpp PROPERTIES GENERATED TRUE )

############################ SET LIBRARIES TO LINK WITH
# Add any 3rd party libraries to link each target with here
find_package(Boost COMPONENTS system program_options thread serialization regex unit_test_framework REQUIRED)
find_package(ZLIB    REQUIRED)
find_package(BZip2   REQUIRED)
find_package(Threads REQUIRED)
SET ( RUNSWIFT_BOOST  ${Boost_SYSTEM_LIBRARY}
                      ${Boost_REGEX_LIBRARY}
                      ${Boost_THREAD_LIBRARY}
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )

TARGET_LINK_LIBRARIES( rssoccer  ${CMAKE_THREAD_LIBS_INIT} ${RUNSWIFT_BOOST} ${PYTHON_LIBRARY} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES})

#add_library(
#        rswalk2014
#        STATIC
#        ${RSWALK_SRCS} )


#ADD_CUSTOM_TARGET ( uncrustify
#   COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../bin/uncrust ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../utils/uncrustify.cfg
#   COMMENT "Running uncrustify"
#)
