#ifndef LEDBLOCK_
#define LEDBLOCK_

#include <iostream>

#include "MemoryBlock.h"
#include <common/RobotInfo.h>

struct LEDBlock : public MemoryBlock {
public:
  LEDBlock()  {
    header.version = 1;
    header.size = sizeof(LEDBlock);
    
    for (int i=0; i<NUM_LEDS; i++) {
      values_[i]=0.0;
    }
    send_leds_ = true;
  }
  bool send_leds_;
  float values_[NUM_LEDS];
};

#endif 
