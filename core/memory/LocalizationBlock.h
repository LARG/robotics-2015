#pragma once

#include <Eigen/Core>
#include <memory/MemoryBlock.h>
#include <math/Geometry.h>
#include <localization/Particle.h>
#define STATE_SIZE 2

class Particle;

struct LocalizationBlock : public MemoryBlock {
  public:
    LocalizationBlock();
    Point2D player;
    Eigen::Matrix<float, STATE_SIZE, 1, Eigen::DontAlign> state;
    Eigen::Matrix<float, STATE_SIZE, STATE_SIZE, Eigen::DontAlign> covariance;
    Point2D getBallPosition();
    Point2D getBallVel();
    Eigen::Matrix2f getBallCov();
    std::vector<Particle> particles;
    void serialize(StreamBuffer& buffer, std::string);
    bool deserialize(const StreamBuffer& buffer, std::string);
};
