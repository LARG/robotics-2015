#pragma once

#include <common/RobotInfo.h>
#include <common/YamlConfig.h>

class Keyframe : public YamlConfig {
  public:
    Keyframe() = default;
    Keyframe(std::string name);
    void deserialize(const YAML::Node& node);
    void serialize(YAML::Emitter& emitter) const;

    float joints[NUM_JOINTS];
    std::string name;
    int delay;
};

class KeyframeSequence : public YamlConfig {
  public:
    void deserialize(const YAML::Node& node);
    void serialize(YAML::Emitter& emitter) const;

    std::vector<Keyframe> keyframes;
};
