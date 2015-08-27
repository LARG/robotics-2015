#ifndef JOINTBLOCK_
#define JOINTBLOCK_

#include <iostream>

#include <common/RobotInfo.h>

#include "MemoryBlock.h"

struct JointBlock : public MemoryBlock {
public:
  JointBlock() {
    header.version = 2;
    header.size = sizeof(JointBlock);
    for (int i=0; i<NUM_JOINTS; i++) {
      values_[i] = 0;
    }
  }

  float getJointValue(int i) const {
    return values_[i];
  }

  float getJointDelta(int i) const {
    return values_[i] - prevValues_[i];
  }
  
  float prevValues_[NUM_JOINTS];
  float values_[NUM_JOINTS];
  float stiffness_[NUM_JOINTS];

};

#endif
