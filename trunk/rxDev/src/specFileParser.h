#ifndef SPECFILEPARSER
#define SPECFILEPARSER
#include <QString>
#include <QStringList>
#include <QDebug>
#include <yaml-cpp/yaml.h>


//Datastructures for specfile data
class Topic_Type{
public:
    std::string topic;
    std::string topictype;
    std::string topiccomment;
} ;

class Name_Type_Default{
public:
    std::string paramName;
    std::string paramType;
    std::string paramDefault;
    std::string paramRange;
    std::string paramcomment;
} ;

class Specfile {
public:
    std::string type;
    std::string package;
    std::list<Topic_Type> subscriptions;
    std::list<Topic_Type> publications;
    std::list<Topic_Type> services;
    std::list<Name_Type_Default> parameters;
};
inline void operator >> (const YAML::Node& node, Specfile& spec) {

}


class SpecFileParser
{
public:
    void nodeParser(const QString nodeFile);
    Specfile node;
    QString writeSpecFile(Specfile *node);

};



#endif // SPECFILEPARSER
