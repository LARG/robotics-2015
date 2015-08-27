#include <common/YamlConfig.h>

std::string YamlConfig::toString() const {
  YAML::Emitter emitter;
  emitter << YAML::BeginDoc;
  emitter << YAML::BeginMap;
  serialize(emitter);
  emitter << YAML::EndMap;
  emitter << YAML::EndDoc;
  return emitter.c_str();
}
    
void YamlConfig::saveToFile(std::string file) const {
  std::ofstream output(file.c_str());
  YAML::Emitter emitter;
  emitter << YAML::BeginDoc;
  emitter << YAML::BeginMap;
  serialize(emitter);
  emitter << YAML::EndMap;
  emitter << YAML::EndDoc;
  output << emitter.c_str();
}

bool YamlConfig::loadFromFile(std::string file){
  std::ifstream input(file.c_str());
  if(!input.good()) {
    printf("Config file %s doesn't exist, ignoring.\n", file.c_str());
    return false;
  }
  try {
    YAML::Parser parser(input);
    YAML::Node doc;
    parser.GetNextDocument(doc);
    deserialize(doc);
  }
  catch (YAML::Exception& e) {
    printf("Config file %s invalid, ignoring.\n", file.c_str());
    return false; 
  }
  return true;
}
