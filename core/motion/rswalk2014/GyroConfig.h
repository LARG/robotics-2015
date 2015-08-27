#ifndef GYROCONFIG_H
#define GYROCONFIG_H

#include <common/InterfaceInfo.h>
#include <common/YamlConfig.h>

class GyroConfig : public YamlConfig {

  public:
    GyroConfig();
    void deserialize(const YAML::Node& node);
    void serialize(YAML::Emitter& emitter) const;

    double offsetX;
    double offsetY;
    double calibration_write_time;

};
#endif
