#pragma once

#include <common/YamlConfig.h>
#include <common/RobotInfo.h>

class KeyframeConfig : public YamlConfig {
  public:
    std::string sequence_file;
    SupportBase base;
    
    void deserialize(const YAML::Node& node) {
      YAML_DESERIALIZE(node, sequence_file);
      YAML_DESERIALIZE(node, base_s);
      base = SupportBaseMethods::fromName(base_s);
    }

    void serialize(YAML::Emitter& emitter) const {
      base_s = SupportBaseMethods::getName(base);
      YAML_SERIALIZE(emitter, sequence_file);
      YAML_SERIALIZE(emitter, base_s);
    }

  private:
    mutable std::string base_s;

};
