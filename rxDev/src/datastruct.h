#include "yaml-cpp/yaml.h"
//Datastructure for specfile data
class Topic_Type{
public:
    std::string topic;
    std::string topictype;
} ;

class Name_Type_Default{
public:
    std::string paramName;
    std::string paramType;
    std::string paramDefault;
} ;


//inline void operator >> (const YAML::Node& node, Topic_Type& tt) {
//   node["topic"] >> tt.topic;
//   node["type"] >> tt.topictype;
//}


//inline void operator >> (const YAML::Node& node, Name_Type_Default& ntd) {
//   node["name"] >> ntd.paramName;
//   node["type"] >> ntd.paramType;
//   node["default"] >> ntd.paramDefault;
//}

