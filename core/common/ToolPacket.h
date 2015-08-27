#ifndef TOOL_PACKET_H
#define TOOL_PACKET_H

#include <common/Enum.h>
#include <common/Poses.h>

#define __TOOL_PACKET_DATA_LENGTH 1024

struct ToolPacket {
  static const int DATA_LENGTH;
  ENUM(MessageType,
    None,
    StateInitial,
    StateReady,
    StateSet,
    StatePlaying,
    StatePenalized,
    StateFinished,
    StateTesting,
    StateTestOdometry,
    StateCameraTop,
    StateCameraBottom,
    LogSelect,
    LogBegin,
    LogEnd,
    LogComplete,
    StreamBegin,
    StreamEnd,
    RestartInterpreter,
    SetTopCameraParameters,
    SetBottomCameraParameters,
    GetCameraParameters,
    ResetCameraParameters,
    ManualControl,
    RunBehavior
  );
  MessageType message;
  int frames;
  float interval;
  struct {
    float x, y, theta, time;
    Poses::Stance stance;
  } odom_command;
  char data[__TOOL_PACKET_DATA_LENGTH];
  ToolPacket();
  ToolPacket(MessageType message);
  ToolPacket(MessageType message, std::string data);
};
#endif
