#pragma once

#include <memory/MemoryBlock.h>

#define SAMPLE_RATE 48000
#define SAMPLE_COUNT 4096
#define NUM_CHANNELS 1

struct AudioProcessingBlock : public MemoryBlock {
  public:
    ENUM(AudioState,
      Detecting,
      TrainingNegative,
      TrainingPositive,
      Off
    );
    AudioProcessingBlock() {
      header.version = 2;
      header.size = sizeof(AudioProcessingBlock);
      whistle_heard_frame_ = -10000;
      teammate_heard_frame_ = -10000;
      state_ = AudioState::Off;
    }

	
    int whistle_heard_frame_, teammate_heard_frame_;
    signed short buffer_[NUM_CHANNELS * SAMPLE_COUNT];
    AudioState state_;
    unsigned long long timestamp_;
};
