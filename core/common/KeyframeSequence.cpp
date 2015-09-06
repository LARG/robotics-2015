#include <common/KeyframeSequence.h>

Keyframe::Keyframe(std::string name) : name(name) {
  for(int i = 0; i < NUM_JOINTS; i++)
    joints[i] = 0.0f;
}

void Keyframe::deserialize(const YAML::Node& node) {
  YAML_DESERIALIZE(node, name);
  const auto& jnode = node["joints"];
  for(int i = 0; i < NUM_JOINTS; i++) {
    jnode[JointNames[i]] >> joints[i];
    joints[i] *= DEG_T_RAD;
  }
}

void Keyframe::serialize(YAML::Emitter& emitter) const {
  YAML_SERIALIZE(emitter, name);
  emitter << YAML::Key << "joints" << YAML::Value;
  emitter << YAML::BeginMap;
  for(int i = 0; i < NUM_JOINTS; i++)
    emitter << YAML::Key << JointNames[i] << YAML::Value << joints[i] * RAD_T_DEG;
  emitter << YAML::EndMap;
}

void KeyframeSequence::deserialize(const YAML::Node& node) {
  YAML_DESERIALIZE(node, keyframes);
}

void KeyframeSequence::serialize(YAML::Emitter& emitter) const {
  YAML_SERIALIZE(emitter, keyframes);
}

