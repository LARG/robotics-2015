#ifndef MOTION_SIM_H
#define MOTION_SIM_H

#include <iostream>
#include <vector>
#include <string>

#include <common/RobotInfo.h>
#include <memory/KickRequestBlock.h>
#include <common/RobotDimensions.h>
#include <VisionCore.h>


class Memory;
class BodyModelBlock;
class FrameInfoBlock;
class GraphableBlock;
class JointBlock;
class JointCommandBlock;
class OdometryBlock;
class SensorBlock;
class WalkEngineBlock;
class WalkRequestBlock;
class OdometryBlock;
class WalkParamBlock;
class KickEngineBlock;
class KickModuleBlock;
class KickParamBlock;
class WalkInfoBlock;

class Memory;
class MotionCore;

class MotionSimulation {

 public:
  MotionSimulation(Memory* memory);
  ~MotionSimulation();


  bool processFrame();
  Memory* getMemory();
  void getLuaParameters();
  void updateMemoryBlocks();
  
  VisionCore* vcore_;
  MotionCore* core_;
  Memory* memory_;

  bool use_com_kick_;

  // specific memory blocks
  BodyModelBlock *body_model_;
  FrameInfoBlock *frame_info_;
  GraphableBlock *graph_;
  JointCommandBlock *commands_;
  JointBlock *joint_angles_;
  OdometryBlock *odometry_;
  SensorBlock *sensors_;
  WalkEngineBlock *walk_engine_mem_;
  WalkRequestBlock *walk_request_;
  WalkParamBlock* walk_params_;
  KickEngineBlock *kick_engine_mem_;
  KickModuleBlock *kick_module_mem_;
  KickParamBlock* kick_params_;
  KickRequestBlock *kick_request_;
  WalkInfoBlock *walk_info_;

  int getUpSide;
  void setWalkRequest(bool walk, float x, float y, float rot);
  void incrWalkRequest(bool walk, float x, float y, float rot);
  void setKickRequest(Kick::Type kickType, Kick::Leg leg, float kickHeading, float kickDistance);

 private:
  RobotDimensions dimensions_;

  // to keep track of commands that take longer than 1 motion frame
  float angle_commands[NUM_JOINTS];
  float angle_time;

  void updateInputs();
  bool updateOutputs();

};


#endif
