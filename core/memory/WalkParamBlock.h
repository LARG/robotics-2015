#ifndef WALKPARAMBLOCK_REVJHZ5T
#define WALKPARAMBLOCK_REVJHZ5T

#include "MemoryBlock.h"
#include <motion/BHWalkParameters.h>

struct WalkParamBlock : public MemoryBlock {
  WalkParamBlock():
    send_params_(false)
  {
    header.version = 11;
    header.size = sizeof(WalkParamBlock);
  }

  bool send_params_;
  BHWalkParameters bh_params_;
  float sagittal_prior_mean;
  float sagittal_prior_stddev;
  float coronal_prior_mean;
  float coronal_prior_stddev;
  float dir_change_wait_duration;
  float turn_limit;
  float walking_turn_limit;

  float walkHeight() { return 400; } //TODO: Put in an actual computation for this
};

#endif /* end of include guard: WALKPARAMBLOCK_REVJHZ5T */
