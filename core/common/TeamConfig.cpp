#include <common/TeamConfig.h>

TeamConfig::TeamConfig() {
}

void TeamConfig::deserialize(const YAML::Node& node) {
  YAML_DESERIALIZE(node, common_ip);
  YAML_DESERIALIZE(node, audio_enabled);
  YAML_DESERIALIZE(node, robot_configs);
}

void TeamConfig::serialize(YAML::Emitter& emitter) const {
  YAML_SERIALIZE(emitter, common_ip);
  YAML_SERIALIZE(emitter, audio_enabled);
  YAML_SERIALIZE(emitter, robot_configs);
}
