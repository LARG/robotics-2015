#ifndef YAML_CONFIG_H
#define YAML_CONFIG_H

#define YAML_SERIALIZE(emitter,x) emitter << YAML::Key << #x << YAML::Value << x
#define YAML_DESERIALIZE(node,x) node[#x] >> x
#define YAML_DESERIALIZE_PTR(node,x) \
  if(const YAML::Node *child = node.FindValue(#x)) { \
    x = new std::remove_pointer<decltype(x)>::type(); \
    *child >> *x; \
  }
#define YAML_SERIALIZE_PTR(emitter,x) \
  if(x) \
    emitter << YAML::Key << #x << YAML::Value << *x;

#include <yaml-cpp/yaml.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

class YamlConfig {
  public:
    void saveToFile(std::string file) const;
    bool loadFromFile(std::string file);
    void save(std::string file) const { return saveToFile(file); }
    bool load(std::string file) { return loadFromFile(file); }
    std::string toString() const;

    friend YAML::Emitter& operator<<(YAML::Emitter& out, const YamlConfig& config) {
      out << YAML::BeginMap;
      config.serialize(out);
      out << YAML::EndMap;
    }

    friend void operator>>(const YAML::Node& node, YamlConfig& config) {
      config.deserialize(node);
    }
  protected:
    virtual void deserialize(const YAML::Node& node) = 0;
    virtual void serialize(YAML::Emitter& emitter) const = 0;
};

#endif
