#ifndef BEHAVIOR_MODULE_H
#define BEHAVIOR_MODULE_H

#include <Module.h>

#include <memory/Memory.h>
#include <math/Geometry.h>

class WorldObjectBlock;
class OpponentBlock;
class BehaviorBlock;
class BehaviorParamBlock;
class RobotStateBlock;
class GameStateBlock;
class FrameInfoBlock;

class BehaviorModule : public Module {

  public:
    void specifyMemoryDependency();
    void specifyMemoryBlocks();
};

#endif
