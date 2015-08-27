#ifndef ROBOTSTATE_
#define ROBOTSTATE_

#include "MemoryBlock.h"
#include <common/States.h>
#include <common/Roles.h>
#include <math/Pose2D.h>

// Man this is a crappy block (but I don't have a great answer)
// ... hopefully we can find somewhere else for it or add something more to the block


struct RobotStateBlock : public MemoryBlock {
public:
  RobotStateBlock();
  int WO_SELF;
  int global_index_; // Normally WO_SELF, except when we're simulating opponents

  int team_;
  bool team_changed_;

  int robot_id_;  // Which robot serial number are we, -1 = unknown or sim

  int role_;

  bool ignore_comms_;
  double clock_offset_;
  Pose2D manual_pose_;
  float manual_height_;

  int head_version_, body_version_;
  char body_id_[16+1];

  std::string bodyId() { return std::string(body_id_).substr(0, 15); }
};

#endif 
