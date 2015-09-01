#pragma once
#include <memory/MemoryBlock.h>

struct TeamPacketsBlock : public MemoryBlock {
  public:
    TeamPacketsBlock() {
      header.version = 0;
      header.size = sizeof(TeamPacketsBlock);
    }
};
