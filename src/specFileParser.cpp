#include "specFileParser.h"
#include <fstream>
#include "ros/ros.h"
#include <QDebug>




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
    //const YAML::Node *yamlNode;

    //kill old data
    node.package.clear();
    node.type.clear();
    node.subscriptions.clear();
    node.publications.clear();
    node.services.clear();
    node.parameters.clear();
    try {
        doc["type"] >> node.type;
        doc["package"] >> node.package;
    } catch(YAML::ParserException& e) {
        std::cout << e.what() << "\n";
    }
    if(doc.FindValue("subscriptions")) {
        for(YAML::Iterator it=doc.FindValue("subscriptions")->begin();it!=doc.FindValue("subscriptions")->end();++it) {
            try {
                const YAML::Node& subs = *it;

                Topic_Type tt;
                subs["topic"]>> tt.topic;
                subs["type"]>> tt.topictype;
                node.subscriptions.push_back(tt);
            } catch(YAML::ParserException& e) {
                std::cout << e.what() << "\n";
            }
        }
    }
    if(doc.FindValue("publications")) {
        for(YAML::Iterator it=doc.FindValue("publications")->begin();it!=doc.FindValue("publications")->end();++it) {
            try {
                const YAML::Node& pubs = *it;
                Topic_Type tt;
                pubs["topic"]>> tt.topic;
                pubs["type"]>> tt.topictype;
                node.publications.push_back(tt);
            } catch(YAML::ParserException& e) {
                std::cout << e.what() << "\n";
            }
        }
    }
    if ((doc.FindValue("services"))) {
        for(YAML::Iterator it=doc.FindValue("services")->begin();it!=doc.FindValue("services")->end();++it) {
            try{
                const YAML::Node& servs = *it;
                Topic_Type tt;
                servs["name"]>> tt.topic;
                servs["type"]>> tt.topictype;
                node.services.push_back(tt);
            } catch(YAML::ParserException& e) {
                std::cout << e.what() << "\n";
            }
        }
    }
    if(doc.FindValue("parameters")) {
        for(YAML::Iterator it=doc.FindValue("parameters")->begin();it!=doc.FindValue("parameters")->end();++it) {
            const YAML::Node& params = *it;
            Name_Type_Default ntd;
            params["name"]>> ntd.paramName;
            params["type"]>> ntd.paramType;
            try{
                if (params.FindValue("range"))
                    params["range"]>> ntd.paramRange;
                else
                    ntd.paramRange= "";
                if (params.FindValue("default"))
                    params["default"]>> ntd.paramDefault;
                else
                    ntd.paramDefault= "";

                params["default"]>> ntd.paramDefault;
            } catch(YAML::ParserException& e) {
                //std::cout << e.what() << "\n";
            } catch (YAML::TypedKeyNotFound <std::string>& e) {
                //std::cout << "key not found: " << e.key << "\n";
            }
            node.parameters.push_back(ntd);
        }
    }


}

QString SpecFileParser::writeSpecFile(Specfile *node)
{
    ///@todo change to sequence for subs, pubs, servs, and params in specfiles, will give more features, but means a lot of changes
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << node->type;
    out << YAML::Key << "package" << YAML::Value << node->package;

    if (!node->subscriptions.size()==0){
        out << YAML::Key << "subscriptions" << YAML::Value << YAML::BeginSeq;
        for(std::list<Topic_Type>::iterator iter=node->subscriptions.begin();iter != node->subscriptions.end();iter++ )
        {
            std::string tempTT;
            tempTT= (Topic_Type(*iter).topic);
            out << YAML::BeginMap;
            out << YAML::Key << "topic" << YAML::Value << tempTT;
            tempTT= (Topic_Type(*iter).topiccomment);
            out << YAML::Comment(tempTT);
            tempTT= (Topic_Type(*iter).topictype);
            out << YAML::Key << "type" << YAML::Value <<tempTT;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
    }
    if (!node->publications.size()==0){
        out << YAML::Key << "publications" << YAML::Value << YAML::BeginSeq;
        for(std::list<Topic_Type>::iterator iter=node->publications.begin();iter != node->publications.end();iter++ )
        {
            std::string tempTT;
            tempTT= (Topic_Type(*iter).topic);
            out << YAML::BeginMap;
            out << YAML::Key << "topic" << YAML::Value << tempTT;
            tempTT= (Topic_Type(*iter).topiccomment);
            out << YAML::Comment(tempTT);
            tempTT= (Topic_Type(*iter).topictype);
            out << YAML::Key << "type" << YAML::Value <<tempTT;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
    }
    if (!node->services.size()==0){
        out << YAML::Key << "services" << YAML::Value << YAML::BeginSeq;
        for(std::list<Topic_Type>::iterator iter=node->services.begin();iter != node->services.end();iter++ )
        {
            std::string tempTT;
            tempTT= (Topic_Type(*iter).topic);
            out << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << tempTT;
            tempTT= (Topic_Type(*iter).topiccomment);
            out << YAML::Comment(tempTT);
            tempTT= (Topic_Type(*iter).topictype);
            out << YAML::Key << "type" << YAML::Value <<tempTT;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
    }
    if (!node->parameters.size()==0){
        out << YAML::Key << "parameters" << YAML::Value << YAML::BeginSeq;
        for(std::list<Name_Type_Default>::iterator iter=node->parameters.begin();iter != node->parameters.end();iter++ )
        {
            std::string tempNTD;
            out << YAML::BeginMap;
            tempNTD= (Name_Type_Default(*iter).paramName);
            out << YAML::Key << "name" << YAML::Value << tempNTD;
            tempNTD= (Name_Type_Default(*iter).paramcomment);
            out << YAML::Comment(tempNTD);
            tempNTD= (Name_Type_Default(*iter).paramType);
            out << YAML::Key << "type" << YAML::Value <<tempNTD;
            tempNTD= (Name_Type_Default(*iter).paramDefault);
            if (tempNTD!="~"&&tempNTD!="")
                out << YAML::Key << "default" << YAML::Value <<tempNTD;
            tempNTD= (Name_Type_Default(*iter).paramRange);
            if (tempNTD!="~"&&tempNTD!="")
                out << YAML::Key << "range" << YAML::Value <<tempNTD;

            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
    }

    out << YAML::EndMap;

    return QString(out.c_str());

}

