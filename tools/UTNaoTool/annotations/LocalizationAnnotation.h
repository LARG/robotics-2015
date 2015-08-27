#pragma once

#include "BaseAnnotation.h"
#include <map>
#include <math/Pose2D.h>

class LocalizationAnnotation : public BaseAnnotation {
  public:
    LocalizationAnnotation() = default;
    LocalizationAnnotation(Pose2D pose, int frame);
    const Pose2D& pose() const { return pose_; }
    int frame() const { return frame_; }
  private:
    Pose2D pose_;
    int frame_;
  protected:
    void serialize(YAML::Emitter& emitter) const;
    void deserialize(const YAML::Node& node);
};
