#pragma once

#include <Module.h>

class OppModule: public Module {
  public:
    void specifyMemoryDependency();
    void specifyMemoryBlocks();
    void initSpecificModule();
    void reInit();

    void processFrame();
};
