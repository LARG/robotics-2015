#ifndef LOG_METADATA_H
#define LOG_METADATA_H

#include <map>
#include <common/YamlConfig.h>

class LogMetadata : public YamlConfig {
  public:
    unsigned int frames;
    std::vector<unsigned int> offsets;
  protected:
    void deserialize(const YAML::Node& node);
    void serialize(YAML::Emitter& emitter) const;
};

#endif
    



