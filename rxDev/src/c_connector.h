
#ifndef SPEC_PARSER_H
#define SPEC_PARSER_H

#include <iostream>
#include <fstream>


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

#endif
