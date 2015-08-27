#ifndef LOCALIZATIONBLOCK_
#define LOCALIZATIONBLOCK_

#include <Eigen/Core>
#include <common/Enum.h>
#include <memory/MemoryBlock.h>
#include <math/Geometry.h>
#include <math/Pose2D.h>
#include <memory/WorldObjectBlock.h>
#define STATE_SIZE 10

#define MAX_MODELS_IN_MEM 4

struct Sides {
  enum SideType {
    NOSIDES,
    LEFTSIDE,
    RIGHTSIDE,
    NUM_SIDES
  };
};

struct Spawns {
  ENUM(SpawnType,
    Ambiguity,
    MultiObject,
    InitialState,
    ReadyState,
    SetState,
    PenalizedState,
    BallFlip,
    Fallen,
    PenaltyKick,
    CenterCircle,
    GoalLines,
    Throwouts
  );
};

struct LocalizationBlock : public MemoryBlock {
public:
  LocalizationBlock();
  int blueSide;

  int kfType;
  int bestModel;
  float bestAlpha;

  // indicate if there are more with significant likelihood that think we're facing the opposite way
  bool oppositeModels;
  bool fallenModels;
  int numMateFlippedBalls;
  int numBadBallUpdates;

  bool useSR;
  int spawnFrames[Spawns::NUM_SpawnTypes];
  int modelNumber[MAX_MODELS_IN_MEM];
  float alpha[MAX_MODELS_IN_MEM];
  float factor;
  Eigen::Matrix<float, STATE_SIZE, 1, Eigen::DontAlign> state[MAX_MODELS_IN_MEM];
  Eigen::Matrix<float, STATE_SIZE, STATE_SIZE, Eigen::DontAlign> covariance[MAX_MODELS_IN_MEM];
  Eigen::Matrix<float, 2, 2, Eigen::DontAlign> ballSR;
  Eigen::Matrix<float, 2, 2, Eigen::DontAlign> objectCov[NUM_WORLD_OBJS];

  Pose2D getPlayerPose(int model);
  Point2D getBallPosition(int model);
  Point2D getBallVelocity(int model);
  Eigen::Matrix2f getPlayerCov(int model);
  Eigen::Matrix2f getBallCov(int model);
  float getOrientationVar(int model);
};

#endif 
