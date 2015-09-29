#include <memory/LocalizationBlock.h>

using namespace Eigen;

LocalizationBlock::LocalizationBlock() {
  header.version = 9;
  header.size = sizeof(LocalizationBlock);
  state = decltype(state)::Zero();
  covariance = decltype(covariance)::Identity();
}

Point2D LocalizationBlock::getBallPosition() {
  return Point2D(state[0], state[1]);
}

Point2D LocalizationBlock::getBallVel() {
  return Point2D(/* fill this in */);
}

Matrix2f LocalizationBlock::getBallCov() {
  return covariance.block<2,2>(0,0);
}
