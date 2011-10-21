
#ifndef SPEC_PARSER_H
#define SPEC_PARSER_H

#include <iostream>
#include <fstream>
#include <QPoint>

//Datastructure
class rosNode {

public:
    QString nodeType;
    QString nodePackage;
    QStringList nodeInput;
    QStringList nodeOutput;
    QStringList nodeServices;
    QStringList nodeParameters;

};

class remapArrowData {

public:
    QPointF startNode;
    QPointF endNode;
    QString from;
    QString to;
    QString ifString;
    QString unlessString;


};

#endif
