#include <common/RobotConfig.h>
#include <common/WorldObject.h>

RobotConfig::RobotConfig() {
  self = role = WO_TEAM_LAST;
  team = 1;
  robot_id = 0;
  posX = posY = posZ = orientation = 0;
  team_udp = 10101;
  team_broadcast_ip = "192.168.1.255";
  walk_type = getName(RUNSWIFT2014_WALK);
  team = 1;
  audio_enabled = false;
}

void RobotConfig::deserialize(const YAML::Node& node) {
  YAML_DESERIALIZE(node, robot_id);
  YAML_DESERIALIZE(node, team);
  YAML_DESERIALIZE(node, self);
  YAML_DESERIALIZE(node, role);
  YAML_DESERIALIZE(node, posX);
  YAML_DESERIALIZE(node, posY);
  YAML_DESERIALIZE(node, posZ);
  YAML_DESERIALIZE(node, orientation);
  YAML_DESERIALIZE(node, team_udp);
  YAML_DESERIALIZE(node, team_broadcast_ip);
  YAML_DESERIALIZE(node, walk_type);
  YAML_DESERIALIZE(node, audio_enabled);
}

void RobotConfig::serialize(YAML::Emitter& emitter) const {
  YAML_SERIALIZE(emitter, robot_id);
  YAML_SERIALIZE(emitter, team);
  YAML_SERIALIZE(emitter, self);
  YAML_SERIALIZE(emitter, role);
  YAML_SERIALIZE(emitter, posX);
  YAML_SERIALIZE(emitter, posY);
  YAML_SERIALIZE(emitter, posZ);
  YAML_SERIALIZE(emitter, orientation);
  YAML_SERIALIZE(emitter, team_udp);
  YAML_SERIALIZE(emitter, team_broadcast_ip);
  YAML_SERIALIZE(emitter, walk_type);
  YAML_SERIALIZE(emitter, audio_enabled);
}
