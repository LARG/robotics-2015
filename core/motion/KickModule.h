#pragma once

#include <Module.h>
#include <common/RobotInfo.h>
#include <memory/MemoryCache.h>

class Keyframe;
class KeyframeSequence;

class KickModule : public Module {
  public:
    KickModule();
    void initSpecificModule();
    void specifyMemoryDependency();
    void specifyMemoryBlocks();

    void processFrame();
    void start();
    bool finished();

  protected:
    ENUM(KickState,
      Initial,
      Running,
      Finished
    );
    void moveToInitial(const Keyframe& keyframe);
    bool reachedKeyframe(const Keyframe& keyframe);
    void moveBetweenKeyframes(const Keyframe& start, const Keyframe& finish, int cframe);
  private:
    KickState state_;
    MemoryCache cache_;
    KeyframeSequence* sequence_;
    int frames_;
    int keyframe_;
};
