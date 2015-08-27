#ifndef TEAM_CONFIG_H
#define TEAM_CONFIG_H

#include <common/RobotConfig.h>
#include <map>

class TeamConfig : public YamlConfig {
  public:
    TeamConfig();
    void deserialize(const YAML::Node& node);
    void serialize(YAML::Emitter& emitter) const;

    std::map<int,RobotConfig> robot_configs;
    std::string common_ip;
    bool audio_enabled;
};
#endif
