#include "specFileParser.h"
#include <fstream>
#include "rxdev.h"
/**
  * Nodespecfile-Parser
  **/
void SpecFileParser::nodeParser(QString nodeFile){

    std::string fname= nodeFile.toStdString();
    std::ifstream fin(fname.c_str());
    if (!fin.is_open()){
        ROS_ERROR("could not open %s.", fname.c_str());
        exit(-1);
    }
    YAML::Parser parser(fin);
    YAML::Node doc;
    parser.GetNextDocument(doc);
    const YAML::Node *yamlNode;

    //kill old data
    node.package.clear();
    node.type.clear();
    node.subscriptions.clear();
    node.publications.clear();
    node.services.clear();
    node.parameters.clear();

    //temp
    std::string nodePackage;
    std::string nodeType;
    std::string nodeInput;
    std::string nodeOutput;
    std::string nodeServices;
    std::string nodeParameters;


    try {
        doc["type"] >> nodeType;
        node.type = QString::fromStdString(nodeType);

    } catch (YAML::InvalidScalar) {
        qDebug()<<"No valid type found";
        exit(-1);
    }
    if ((yamlNode = doc.FindValue("package"))) {
        *yamlNode >> nodePackage;
        node.package = QString::fromStdString(nodePackage);
    } else{
        qDebug()<<"No valid package found in "+nodeFile;

    }
    if ((yamlNode = doc.FindValue("subscriptions"))) {

        *yamlNode >> nodeInput;
        if (nodeInput!="~"){

            node.subscriptions = QString::fromStdString(nodeInput).split(" ");
        }
    }
    if ((yamlNode = doc.FindValue("publications"))) {
        *yamlNode >> nodeOutput;
        if (nodeOutput!="~"){
            node.publications = QString::fromStdString(nodeOutput).split(" ");
        }

    }
    if ((yamlNode = doc.FindValue("services"))) {
        *yamlNode >> nodeServices;
        if (nodeServices!="~"){
            node.services = QString::fromStdString(nodeServices).split(" ");
        }

    }
    if ((yamlNode = doc.FindValue("parameters"))) {
        *yamlNode >> nodeParameters;
        if (nodeParameters!="~"){
            node.parameters = QString::fromStdString(nodeParameters).split(" ");
        }
    }

}

QString SpecFileParser::writeSpecFile(rosNode *node)
{
    ///@todo change to sequence for subs, pubs, servs, and params in specfiles, will give more features, but means a lot of changes
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "type";
    out << YAML::Value << node->type.toStdString();
    out << YAML::Key << "package";
    out << YAML::Value << node->package.toStdString();
    //        if (!specFile.getSubscriptions().count()==0){
    //        out << YAML::Key << "subscriptions"<< YAML::Value;
    //        for (int i;i<specFile.getSubscriptions().count();i++)
    //            out << specFile.getSubscriptions().at(i).toStdString();
    //        }

    //        out << YAML::Key << "publications";

    //        for (int i;i<specFile.getPublications().count();i++)
    //            out << YAML::Value <<specFile.getPublications().at(i).toStdString();


    //        out << YAML::Key << "services";

    //        for (int i;i<specFile.getServices().count();i++)
    //            out << YAML::Value <<specFile.getServices().at(i).toStdString();

    //        out << YAML::Key << "parameters";
    //        for (int i;i<specFile.getParameters().count();i++)
    //            out << YAML::Value <<specFile.getParameters().at(i).toStdString();
    out << YAML::EndMap;

    //move: new SOlution please in writeSpecFile
    QString tempContents;
    if (!node->subscriptions.count()==0){
        tempContents.append("subscriptions:");
        for (int i=0;i<node->subscriptions.count();i++)
            tempContents.append("\n  "+node->subscriptions.at(i));
        tempContents.append("\n");
    }

    if (!node->publications.count()==0){
        tempContents.append("publications:");
        for (int i=0;i<node->publications.count();i++)
            tempContents.append("\n  "+node->publications.at(i));
        tempContents.append("\n");
    }
    if (!node->services.count()==0){
        tempContents.append("services:");
        for (int i=0;i<node->services.count();i++)
            tempContents.append("\n  "+node->services.at(i));
        tempContents.append("\n");
    }
    if (!node->parameters.count()==0){
        tempContents.append("parameters:");
        for (int i=0;i<node->parameters.count();i++)
            tempContents.append("\n  "+node->parameters.at(i));
        tempContents.append("\n");
    }
    //end move


    //qDebug()<< QString(out.c_str());

    //        QString strin;
    //                out.Write(strin);
    //        qDebug()<< QString(strin);
    QString output = QString(out.c_str()).append("\n"+tempContents);
    return output;

}

