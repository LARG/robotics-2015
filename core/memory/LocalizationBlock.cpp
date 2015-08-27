#include <memory/LocalizationBlock.h>

using namespace Eigen;

LocalizationBlock::LocalizationBlock() {
  header.version = 9;
  header.size = sizeof(LocalizationBlock);
  bestModel = 0;
  for (int i = 0; i < MAX_MODELS_IN_MEM; i++){
    modelNumber[i] = i;
    alpha[i] = 0;
  }
  bestAlpha = 0;
  oppositeModels = false;
  fallenModels = false;
  numMateFlippedBalls = 0;
  numBadBallUpdates = 0;
  blueSide = 0;
  factor = 0;
}

Pose2D LocalizationBlock::getPlayerPose(int model) {
  Pose2D pose;
  pose.translation.x = state[model][0];
  pose.translation.y = state[model][1];
  pose.rotation = state[model][2];
  pose.translation *= factor;
  return pose;
}

Point2D LocalizationBlock::getBallPosition(int model) {
  Point2D point;
  point.x = state[model][3];
  point.y = state[model][4];
  point *= factor;
  return point;
}

Point2D LocalizationBlock::getBallVelocity(int model) {
  Point2D point;
  point.x = state[model][5];
  point.y = state[model][6];
  point *= factor;
  return point;
}

Matrix2f LocalizationBlock::getPlayerCov(int model) {
  Matrix2f cov;
  auto& full = covariance[model];
  cov << full.block<2,2>(0,0);
  cov *= factor * factor;
  return cov;
}

Matrix2f LocalizationBlock::getBallCov(int model) {
  if(useSR)
    return ballSR * factor;
  
  Matrix2f cov;
  auto& full = covariance[model];
  cov << full.block<2,2>(3,3);
  cov *= factor;
  return cov;
}

float LocalizationBlock::getOrientationVar(int model) {
  auto& full = covariance[model];
  return full(2,2) * full(2,2);
}
