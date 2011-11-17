#ifndef SPECFILEPARSER
#define SPECFILEPARSER
#include <QString>
#include <QStringList>
#include <yaml-cpp/yaml.h>

//Datastructure for specfile data
class rosNode {
public:
    QString type;
    QString package;
    QStringList subscriptions;
    QStringList publications;
    QStringList services;
    QStringList parameters;
};

class SpecFileParser
{
public:
    void nodeParser(const QString nodeFile);
    rosNode node;
    QString writeSpecFile(rosNode *node);

};


#endif // SPECFILEPARSER






