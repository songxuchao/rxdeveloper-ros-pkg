#include "rxdev.h"
#include "ui_rxdev.h"

#include "tagItems/argItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/groupItem.h"
#include "tagItems/remapItem.h"
#include "tagItems/remapArrow.h"
#include "QDateTime"
/*!\brief find <launch> tag
 *
 * Parses the given file to find the important <launch>-tag and start the parsing of the tags
 */
QList<remapArrowData>arrowList;
QStringList groups;

void RxDev::expandInclude(const QString &file, GroupItem &group){

    std::string utf8_text = file.toUtf8().constData();
    TiXmlDocument doc( utf8_text );
    bool loadOkay = doc.LoadFile();
    if (loadOkay)
    {
//        qDebug()<<group.pos();
        if (group.isVisible() && group.pos()!=QPoint(0,0))          //if IncludeItem has a Parent-GroupItem
            loadDocumentInGroup( &doc, group);
        else                            //treat inclue as an standard launchfile
            loadDocument( &doc );
        QTime time=QTime::currentTime();
        ui->textEdit_Info->append("<font color=\"red\">"+time.toString()+" - information: <font color=\"blue\">Includefile expanded");
        ui->textEdit_Info->append("<font color=\"black\">  "+file);
        ui->dockWidget_errors->show();
    }
    else
    {
        qDebug()<<"\nFailed to load file: "<<file;
    }
}
void RxDev::loadDocumentInGroup( TiXmlNode * documentNode, GroupItem &group)
{
    arrowList.clear();
    groups.clear();
    if ( !documentNode ) return;
    int x = group.pos().x();
    int y = group.pos().y();
    TiXmlNode * pChild2;
    for ( pChild2 = documentNode->FirstChild(); pChild2 != 0; pChild2 = pChild2->NextSibling())
    {
        if (QString(pChild2->Value())=="launch"){

            //check for included items
            TiXmlNode * pChild;
            for ( pChild = pChild2->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
            {

                if (QString(pChild->Value())=="env"){
                    int x,y;
                    EnvItem* newEnv = new EnvItem;
                    create_envItem(*newEnv,pChild,x,y);
                    newEnv->setParentItem(&group);
                    scene->addItem(newEnv);

                    if (x==0 &&y==0){
                        newEnv->setPos(((newEnv->mapToParent(QPoint(x,y)))));
                        newEnv->setLocation(newEnv->mapToParent(newEnv->pos()));
                    } else{
                        newEnv->setPos(QPoint(x,y));
                        newEnv->setLocation(newEnv->mapToParent(newEnv->pos()));
                    }
                }else if (QString(pChild->Value())=="param"){
                    int x,y;
                    ParameterItem* newParam = new ParameterItem;
                    create_paramItem(*newParam,pChild,x,y);
                    newParam->setParentItem(&group);
                    scene->addItem(newParam);
                    if (x==0 &&y==0){
                        newParam->setPos(((newParam->mapToParent(QPoint(x,y)))));
                        newParam->setLocation(newParam->mapToParent(newParam->pos()));
                    } else{
                        newParam->setPos(QPoint(x,y));
                        newParam->setLocation(newParam->mapToParent(newParam->pos()));

                    }
                }else if (QString(pChild->Value())=="rosparam"){
                    int x,y;
                    RosparamItem* newRosparam = new RosparamItem;
                    create_rosparamItem(*newRosparam,pChild,x,y);
                    newRosparam->setParentItem(&group);
                    scene->addItem(newRosparam);
                    if (x==0 &&y==0){
                        newRosparam->setPos(((newRosparam->mapToParent(QPoint(x,y)))));
                        newRosparam->setLocation(newRosparam->mapToParent(newRosparam->pos()));
                    } else{
                        newRosparam->setPos(QPoint(x,y));
                        newRosparam->setLocation(newRosparam->mapToParent(newRosparam->pos()));
                    }
                }else if (QString(pChild->Value())=="remap"){
                    int x,y;
                    RemapItem* newRemap = new RemapItem;
                    create_remapItem(*newRemap,pChild,x,y);
                    newRemap->setParentItem(&group);
                    scene->addItem(newRemap);
                    if (x==0 &&y==0){
                        newRemap->setPos((((QPoint(x,y)))));
                        newRemap->setLocation(newRemap->mapToParent(newRemap->pos()));
                    } else{
                        newRemap->setPos(QPoint(x,y));
                        newRemap->setLocation(newRemap->mapToParent(newRemap->pos()));
                    }

                }else if (QString(pChild->Value())=="arg"){
                    int x,y;
                    ArgItem* newArg = new ArgItem;
                    create_argItem(*newArg,pChild,x,y);
                    newArg->setParentItem(&group);
                    scene->addItem(newArg);
                    if (x==0 &&y==0){
                        newArg->setPos(((newArg->mapToParent(QPoint(x,y)))));
                        newArg->setLocation(newArg->mapToParent(newArg->pos()));
                    } else{
                        newArg->setPos(QPoint(x,y));
                        newArg->setLocation(newArg->mapToParent(newArg->pos()));
                    }
                }
                else if (QString(pChild->Value())=="include"){
                    int x,y;
                    IncludeFileItem* newIncludeFile = new IncludeFileItem;
                    create_includeFileItem(*newIncludeFile,pChild,x,y);
                    newIncludeFile->setParentItem(&group);
                    scene->addItem(newIncludeFile);
                    if (x==0 &&y==0){
                        newIncludeFile->setPos((newIncludeFile->mapToParent(QPoint(x,y))));
                        newIncludeFile->setLocation(newIncludeFile->mapToParent(newIncludeFile->pos()));
                    } else{
                        newIncludeFile->setPos(QPoint(x,y));
                        newIncludeFile->setLocation(newIncludeFile->mapToParent(newIncludeFile->pos()));
                    }
                    connect(newIncludeFile,SIGNAL(expandItem(QString,GroupItem &)),this,SLOT(expandInclude(const QString &,GroupItem &)));


                }else if (QString(pChild->Value())=="machine"){
                    int x,y;
                    MachineItem* newMachine = new MachineItem;
                    create_machineItem(*newMachine,pChild,x,y);
                    newMachine->setParentItem(&group);
                    scene->addItem(newMachine);
                    if (x==0 &&y==0){
                        newMachine->setPos(((newMachine->mapToParent(QPoint(x,y)))));
                        newMachine->setLocation(newMachine->mapToParent(newMachine->pos()));
                    } else{
                        newMachine->setPos(QPoint(x,y));
                        newMachine->setLocation(newMachine->mapToParent(newMachine->pos()));
                    }
                }else if (QString(pChild->Value())=="node"){
                    NodeItem *newNode;
                    int x1=x,y1=y;
                    QString nodePackage, nodeType;
                    QStringList nodeSubs, nodePubs, nodeParams, nodeSrvs;
                    prepare_nodeOrTest(pChild, nodePackage, nodeType,
                                       nodeSubs,nodePubs, nodeParams, nodeSrvs);
                    newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeParams);
                    create_nodeorTestItem(*newNode,0, pChild, x1 , y1);
                    newNode->setParentItem(&group);
                    scene->addItem(newNode);

                    if (x==0 &&y==0){
                        newNode->setPos(QPoint(x1,y1));
                        newNode->setLocation(newNode->mapToParent(newNode->pos()));
                    }else{
                        newNode->setPos(QPoint(x1,y1));
                        newNode->setLocation(newNode->mapToParent(newNode->pos()));
                    }

                }else if (QString(pChild->Value())=="test"){
                    NodeItem *newNode;
                    int x1=x,y1=y;
                    QString nodePackage, nodeType;
                    QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
                    prepare_nodeOrTest(pChild, nodePackage, nodeType,
                                       nodeSubs,nodePubs, nodeArgs, nodeSrvs);
                    newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
                    create_nodeorTestItem(*newNode,1, pChild,x,y);
                    newNode->setParentItem(&group);
                    scene->addItem(newNode);

                    if (x==0 &&y==0){
                        newNode->setPos(QPoint(x1,y1));
                        newNode->setLocation(newNode->mapToParent(newNode->pos()));
                    }else{
                        newNode->setPos(QPoint(x1,y1));
                        newNode->setLocation(newNode->mapToParent(newNode->pos()));
                    }


                }
            }
        }
        //    qDebug()<<"______________________________________________________";
//        foreach(QGraphicsItem *item, scene->items()){
//            if (item->type()==NodeItem::Type)
//                qDebug()<<item;
//        }
        //    qDebug()<<arrowList.count();
        //    qDebug()<<"______________________________________________________";

        for (int i=0;i<arrowList.count();i++){
            create_remapArrow(&arrowList.at(i));
        }

    }

}


void RxDev::loadDocument( TiXmlNode * documentNode)
{


    arrowList.clear();
    groups.clear();
    if ( !documentNode ) return;
    TiXmlNode * pChild;
    for ( pChild = documentNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {

        if (QString(pChild->Value())=="launch"){
            TiXmlAttribute* tagAttribute=pChild->ToElement()->FirstAttribute();
            while (tagAttribute)
            {

                if (QString(tagAttribute->Name())=="deprecated"){
                    gview->setDeprecated(true);
                    gview->setMessage(QString(tagAttribute->Value()));
                }
            tagAttribute=tagAttribute->Next();

            }
            TiXmlNode * pChild2;
            for ( pChild2 = pChild->FirstChild(); pChild2 != 0; pChild2 = pChild2->NextSibling())
            {

                beginParsing(pChild2);

            }
        }
    }
    //    qDebug()<<"______________________________________________________";
//    foreach(QGraphicsItem *item, scene->items()){
//        if (item->type()==NodeItem::Type)
//            qDebug()<<item;
//    }
    //    qDebug()<<arrowList.count();
    //    qDebug()<<"______________________________________________________";

    for (int i=0;i<arrowList.count();i++){
        create_remapArrow(&arrowList.at(i));
    }



}


/*!\brief First-level file parsing
 *
 * Parses the launch file for tags that are children of the <launch>-tag.
 */
void RxDev::beginParsing(TiXmlNode *firstLevelNode){

    switch ( firstLevelNode->Type() )
    {
    case TiXmlNode::TINYXML_ELEMENT:                        // case with the important <tags>

        if (QString(firstLevelNode->Value())=="machine"){
            MachineItem* newMachine = new MachineItem;
            int x,y;
            create_machineItem(*newMachine,firstLevelNode,x,y);
            scene->addItem(newMachine);
            if (x==0 &&y==0){
                newMachine->setPos((findSpace(newMachine->mapToParent(QPoint(x,y)))));
                newMachine->setLocation((newMachine->pos()));

            } else{
                newMachine->setPos(QPoint(x,y));
                newMachine->setLocation((newMachine->pos()));
            }

        }else if (QString(firstLevelNode->Value())=="group"){
            GroupItem* newGroup;
            newGroup = new GroupItem;
            create_groupItem(firstLevelNode,newGroup);

        }else if (QString(firstLevelNode->Value())=="node"){
            NodeItem *newNode;
            int x,y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(firstLevelNode, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,0, firstLevelNode, x, y);
            scene->addItem(newNode);
            if (x==0 &&y==0){
                newNode->setPos(findSpace(QPoint(x,y)));
                newNode->setLocation(newNode->pos());
            } else{
                newNode->setPos(QPoint(x,y));
                newNode->setLocation((newNode->pos()));
            }
        }else if (QString(firstLevelNode->Value())=="test"){
            NodeItem *newNode;
            int x,y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(firstLevelNode, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,1, firstLevelNode, x, y);
            scene->addItem(newNode);
            if (x==0 &&y==0){
                newNode->setPos(findSpace(QPoint(x,y)));
                newNode->setLocation(newNode->pos());
            } else{
                newNode->setPos(QPoint(x,y));
                newNode->setLocation((newNode->pos()));
            }
        }else if (QString(firstLevelNode->Value())=="param"){
            ParameterItem * newParam = new ParameterItem;
            int x,y;
            create_paramItem(*newParam,firstLevelNode,x,y);
            scene->addItem(newParam);
            if (x==0 &&y==0){
                newParam->setPos(findSpace(QPoint(x,y)));
                newParam->setLocation(newParam->pos());
            } else{
                newParam->setPos(QPoint(x,y));
                newParam->setLocation((newParam->pos()));
            }
        }else if (QString(firstLevelNode->Value())=="rosparam"){
            RosparamItem * newRosparam = new RosparamItem;
            int x,y;
            create_rosparamItem(*newRosparam,firstLevelNode,x,y);
            scene->addItem(newRosparam);
            if (x==0 &&y==0){
                newRosparam->setPos(findSpace(QPoint(x,y)));
                newRosparam->setLocation(newRosparam->pos());
            } else{
                newRosparam->setPos(QPoint(x,y));
                newRosparam->setLocation((newRosparam->pos()));
            }
        }else if (QString(firstLevelNode->Value())=="remap"){
            RemapItem* newRemap = new RemapItem;
            int x,y;
            create_remapItem(*newRemap,firstLevelNode,x,y);
            scene->addItem(newRemap);
            if (x==0 &&y==0){
                newRemap->setPos(findSpace(QPoint(x,y)));
                newRemap->setLocation(newRemap->pos());
            } else{
                newRemap->setPos(QPoint(x,y));
                newRemap->setLocation((newRemap->pos()));
            }
        }else if (QString(firstLevelNode->Value())=="include"){
            IncludeFileItem* newIncludeFile = new IncludeFileItem;
            int x,y;
            create_includeFileItem(*newIncludeFile,firstLevelNode,x,y);
            scene->addItem(newIncludeFile);
            if (x==0 &&y==0){
                newIncludeFile->setPos(findSpace(QPoint(x,y)));
                newIncludeFile->setLocation(newIncludeFile->pos());
            } else{
                newIncludeFile->setPos(QPoint(x,y));
                newIncludeFile->setLocation((newIncludeFile->pos()));
            }
            connect(newIncludeFile,SIGNAL(expandItem(QString,GroupItem &)),this,SLOT(expandInclude(const QString &,GroupItem &)));

        }else if (QString(firstLevelNode->Value())=="env"){
            EnvItem * newEnv = new EnvItem;
            int x,y;
            create_envItem(*newEnv,firstLevelNode,x,y);
            scene->addItem(newEnv);
            if (x==0 &&y==0){
                newEnv->setPos(findSpace(QPoint(x,y)));
                newEnv->setLocation(newEnv->pos());
            } else{
                newEnv->setPos(QPoint(x,y));
                newEnv->setLocation((newEnv->pos()));
            }

        }else if (QString(firstLevelNode->Value())=="arg"){
            int x,y;
            ArgItem * newArg = new ArgItem;
            create_argItem(*newArg,firstLevelNode,x,y);
            scene->addItem(newArg);
            if (x==0 &&y==0){
                newArg->setPos(findSpace(QPoint(x,y)));
                newArg->setLocation(newArg->pos());
            } else{
                newArg->setPos(QPoint(x,y));
                newArg->setLocation((newArg->pos()));
            }
        }
        break;

    case TiXmlNode::TINYXML_COMMENT:{                        // ignored <tag>
        //printf( "Comment: \"%s\"", firstLevelNode->Value());
        //QPoint coords =getCoords(firstLevelNode);
        //qDebug()<<coords;
        break;
    }

    case TiXmlNode::TINYXML_UNKNOWN:                        // ignored <tag>
        printf( "Unknown" );
        break;

    case TiXmlNode::TINYXML_DECLARATION:                    // ignored <tag>
        printf( "Declaration" );
        break;
    default:
        break;
    }
    printf( "\n" );

}

QPoint RxDev::getCoords(TiXmlNode *node){
    int x=0,y=0;
    if (QString(node->Value())!=""){
        QString stringToParse;
        stringToParse = node->Value();
        if (stringToParse.contains("x=\"")){
            int beginn = stringToParse.indexOf("x=\"")+3;
            int end = stringToParse.indexOf("\"", beginn+1);
            if (end!=-1){
                //                qDebug()<< stringToParse.mid(beginn,end-beginn);
                x=atoi((stringToParse.mid(beginn,end-beginn)).toAscii().data());
            }
        }
        if (stringToParse.contains("y=\"")){
            int beginn = stringToParse.indexOf("y=\"")+3;
            int end = stringToParse.indexOf("\"", beginn+1);
            if (end!=-1){
                //                qDebug()<< stringToParse.mid(beginn,end-beginn);
                y=atoi((stringToParse.mid(beginn,end-beginn)).toAscii().data());
            }
        }
    }
    return QPoint(x,y);
}
void RxDev::getGroupDim(TiXmlNode *node, int &width,int &height){
    width=300,height=300;
    if (QString(node->Value())!=""){
        QString stringToParse;
        stringToParse = node->Value();
        if (stringToParse.contains("width=\"")){
            int beginn = stringToParse.indexOf("width=\"")+7;
            int end = stringToParse.indexOf("\"", beginn+1);
            if (end!=-1){
                //                qDebug()<< stringToParse.mid(beginn,end-beginn);
                width=atoi((stringToParse.mid(beginn,end-beginn)).toAscii().data());
            }
        }
        if (stringToParse.contains("height=\"")){
            int beginn = stringToParse.indexOf("height=\"")+8;
            int end = stringToParse.indexOf("\"", beginn+1);
            if (end!=-1){
                //                qDebug()<< stringToParse.mid(beginn,end-beginn);
                height=atoi((stringToParse.mid(beginn,end-beginn)).toAscii().data());
            }
        }
    }
}

/*!\brief <machine>-tag object
 *
 * get attributes for the <machine>-object and search for including tags.
 */
void RxDev::create_machineItem(MachineItem &newMachine, TiXmlNode *machineNode,int &x,int &y)
{
    TiXmlAttribute* tagAttribute=machineNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="name"){
            newMachine.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="address"){
            newMachine.setAddress(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ros-root"){
            newMachine.setRos_root(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ros-package-path"){
            newMachine.setRos_package_path(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="user"){
            newMachine.setUser(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="password"){
            newMachine.setPassword(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="timeout"){
            newMachine.setTimeout(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="default"){
            newMachine.setDefault(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="if"){
            newMachine.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newMachine.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }

    newMachine.updateMachineItem();
    int includeX,includeY;      //needed as fakepositions

    //check for env-items
    TiXmlNode * pChild;
    for ( pChild = machineNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        //printf( "child" );
        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild,includeX,includeY);
            newMachine.addEnvItem(newEnv);
        }else if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
    }
}

/*!\brief collect information for <node>- or <test>-tag object
 *
 * collects some information that are needed to create the node or test object. After that it creates the object.
 */
void RxDev::prepare_nodeOrTest(TiXmlNode *nodeNode,
                               QString &nodePackage, QString &nodeType,QStringList &nodeSubs,
                               QStringList &nodePubs,QStringList &nodeParams,QStringList &nodeSrvs){

    TiXmlAttribute* tagAttribute=nodeNode->ToElement()->FirstAttribute();

    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="type"){
            nodeType =QString(tagAttribute->Value());
        }else if (QString(tagAttribute->Name())=="pkg"){
            nodePackage =QString(tagAttribute->Value());
        }
        tagAttribute=tagAttribute->Next();
    }

    //begin: fetch data, if allready a specfile for the node exists
    QList<QStandardItem *> items = model_availableNodes->findItems(nodeType,Qt::MatchExactly, 0);
    QStandardItem *item;
    for (int i = 0; i < items.size(); i++) {
        item = items.at(i);
        if (item->child(1,0)->child(0,0)->text() ==nodePackage){
            SpecFileParser *specParser = new SpecFileParser;
            specParser->nodeParser(item->child(0,0)->child(0,0)->data(Qt::DisplayRole).toString());

            QStringList subs;
            for(std::list<Topic_Type>::iterator iter=specParser->node.subscriptions.begin();iter != specParser->node.subscriptions.end();iter++ )
            {
                subs.push_back(QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)));
            }
            nodeSubs = subs;
            QStringList pubs;
            for(std::list<Topic_Type>::iterator iter=specParser->node.publications.begin();iter != specParser->node.publications.end();iter++ )
            {
                pubs.push_back(QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)));
            }
            nodePubs = pubs;
            QStringList servs;
            for(std::list<Topic_Type>::iterator iter=specParser->node.services.begin();iter != specParser->node.services.end();iter++ )
            {
                servs.push_back(QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)));
            }
            nodeSrvs = servs;
            QStringList params;
            for(std::list<Name_Type_Default>::iterator iter=specParser->node.parameters.begin();iter != specParser->node.parameters.end();iter++ )
            {
                params.push_back(QString("%1 %2 %3").arg(QString::fromStdString(Name_Type_Default(*iter).paramName)).arg(QString::fromStdString(Name_Type_Default(*iter).paramType)).arg(QString::fromStdString(Name_Type_Default(*iter).paramDefault)));
                //todo add topictype and default anyhow
            }
             nodeParams = params;

            break;
        }
    }
    //end: fetch data, if allready a specfile for the node exists

}

/*!\brief <node>- or <test>-tag object
 *
 * get attributes for the object and search for including tags.
 */
void RxDev::create_nodeorTestItem(NodeItem &newNode, int nodeOrTest, TiXmlNode *nodeOrTestNode,int &x, int &y)
{
    TiXmlAttribute* tagAttribute=nodeOrTestNode->ToElement()->FirstAttribute();
    while (tagAttribute)
    {
        newNode.setNode_or_test(nodeOrTest);
        if (QString(tagAttribute->Name())=="name"){
            newNode.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="test-name"){
            newNode.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="args"){
            newNode.setArgs(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ns"){
            newNode.setNamespace(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="machine"){
            newNode.setMachine(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="time-limit"){
            newNode.setTime_limit(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="retry"){
            newNode.setRetry(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="launch-prefix"){
            newNode.setLaunch_prefix(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="required"){
            if (QString(tagAttribute->Value())=="true")
                newNode.setRequired(1);
            else if (QString(tagAttribute->Value())=="false")
                newNode.setRequired(2);
            else
                newNode.setRequired(0);
        }else if (QString(tagAttribute->Name())=="respawn"){
            if (QString(tagAttribute->Value())=="true")
                newNode.setRespawn(1);
            else if (QString(tagAttribute->Value())=="false")
                newNode.setRespawn(2);
            else
                newNode.setRespawn(0);
        }else if (QString(tagAttribute->Name())=="clear_params"){
            if (QString(tagAttribute->Value())=="true")
                newNode.setClear_params(1);
            else if (QString(tagAttribute->Value())=="false")
                newNode.setClear_params(2);
            else
                newNode.setClear_params(0);
        }else if (QString(tagAttribute->Name())=="output"){
            if (QString(tagAttribute->Value())=="log")
                newNode.setOutput(1);
            else if (QString(tagAttribute->Value())=="screen")
                newNode.setOutput(2);
            else
                newNode.setOutput(0);
        }else if (QString(tagAttribute->Name())=="cwd"){
            if (QString(tagAttribute->Value())=="ROS_HOME")
                newNode.setCwd(1);
            else if (QString(tagAttribute->Value())=="node")
                newNode.setCwd(2);
            else
                newNode.setCwd(0);
        }else if (QString(tagAttribute->Name())=="if"){
            newNode.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newNode.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }

    newNode.updateNodeItem();

    //check for env, param, rosparam and remap items
    TiXmlNode * pChild;
    for ( pChild = nodeOrTestNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        int includeX,includeY;      //needed as fakepositions

        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild,includeX,includeY);
            newNode.addEnvItem(newEnv);

        }else if (QString(pChild->Value())=="param"){
            ParameterItem* newParam = new ParameterItem;
            create_paramItem(*newParam,pChild,includeX,includeY);
            newNode.addParamItem(newParam);
        }else if (QString(pChild->Value())=="rosparam"){
            RosparamItem* newRosparam = new RosparamItem;
            create_rosparamItem(*newRosparam,pChild,includeX,includeY);
            newNode.addRosparamItem(newRosparam);
        }else if (QString(pChild->Value())=="remap"){
            RemapItem* newRemap = new RemapItem;
            TiXmlAttribute* tagAttribute=pChild->ToElement()->FirstAttribute();
            while (tagAttribute)
            {
                if (QString(tagAttribute->Name())=="from"){
                    newRemap->setFrom(QString(tagAttribute->Value()));
                }else if (QString(tagAttribute->Name())=="to"){
                    newRemap->setTo(QString(tagAttribute->Value()));
                }else if (QString(tagAttribute->Name())=="if"){
                    newRemap->setIf(QString(tagAttribute->Value()));
                }else if (QString(tagAttribute->Name())=="unless"){
                    newRemap->setUnless(QString(tagAttribute->Value()));
                }
                tagAttribute=tagAttribute->Next();
            }
            newRemap->updateRemapItem();
            //newNode.addRemapItem(newRemap);
            //            qDebug()<<"_________________________";
            //            qDebug()<<newNode.pos();
            //            qDebug()<<"_________________________";

            remapArrowData *remapData=new remapArrowData;
            remapData->startNode=&newNode;
            remapData->item=newRemap;
            arrowList.append(*remapData);

        } else if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;
    }
}

/*!\brief <parameter>-tag object
 *
 * get attributes for the <parameter>-object.
 */
void RxDev::create_paramItem(ParameterItem &newParam, TiXmlNode *parameterNode,int &x,int &y)
{
    TiXmlAttribute* tagAttribute=parameterNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="name"){
            newParam.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="value"){
            newParam.setStandardParameter(1);
            newParam.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="type"){
            newParam.setType(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="command"){
            newParam.setStandardParameter(2);
            newParam.setType("command");
            newParam.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="textfile"){
            newParam.setStandardParameter(2);
            newParam.setType("textfile");
            newParam.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="binfile"){
            newParam.setStandardParameter(2);
            newParam.setType("binfile");
            newParam.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="if"){
            newParam.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newParam.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }
    TiXmlNode * pChild;
    for ( pChild = parameterNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;
    }

    newParam.updateParameterItem();
}

/*!\brief <remap>-tag object
 *
 * get attributes for the object.
 */
void RxDev::create_remapItem(RemapItem &newRemap,TiXmlNode *remapNode,int &x,int &y)
{
    TiXmlAttribute* tagAttribute=remapNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="from"){
            newRemap.setFrom(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="to"){
            newRemap.setTo(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="if"){
            newRemap.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newRemap.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }
    TiXmlNode * pChild;
    for ( pChild = remapNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;
    }
    newRemap.updateRemapItem();
}

void RxDev::create_remapArrow(const remapArrowData *arrowdata)
{
    QString toString=arrowdata->item->getTo();
    QStringList toStringList=toString.split("/");
    bool foundTarget = false;
    bool targetInGroup = false;
    foreach(QString group,groups){
        foreach(QString string, toStringList){
            //            qDebug()<<group<<":"<<string;
            if (string==group){
                targetInGroup=true;
                //                qDebug()<<"target is in group";
            }
        }
    }
    foreach(QGraphicsItem *item, scene->items()){
        if (item->type()==65540&&item!=arrowdata->startNode){
            NodeItem *targetNode = qgraphicsitem_cast<NodeItem *>(item);
            if (targetInGroup)
            {
                if (targetNode->parentItem()){
                    GroupItem *checkGroupItem =qgraphicsitem_cast<GroupItem *>(targetNode->parentItem());
                    foreach(QString stringNs, toStringList){
                        foreach(QString stringName, toStringList){
                            if(checkGroupItem->getNamespace()==stringNs&&targetNode->getName()==stringName){

                                RemapArrow *arrow = new RemapArrow(arrowdata->startNode, targetNode);
                                arrow->setTo(arrowdata->item->getTo());
                                arrow->setFrom(arrowdata->item->getFrom());
                                arrow->setIf(arrowdata->item->getIf());
                                arrow->setUnless(arrowdata->item->getUnless());
                                arrowdata->startNode->addArrow(arrow);
                                targetNode->addArrow(arrow);
                                arrow->setZValue(-20);
                                scene->addItem(arrow);
                                arrow->updatePosition();
                                foundTarget=true;
                            }
                        }
                    }
                }
            }else{
                foreach(QString string, toStringList){
                    //   qDebug()<<string<<":"<<targetNode->getName();
                    if (!targetNode->parentItem()&&string==targetNode->getName()){


                        //                qDebug()<<"from"<<arrowdata->startNode->pos();
                        //                qDebug()<<"to"<<targetNode->getName();
                        //                qDebug()<<toString;
                        RemapArrow *arrow = new RemapArrow(arrowdata->startNode, targetNode);
                        arrow->setTo(arrowdata->item->getTo());
                        arrow->setFrom(arrowdata->item->getFrom());
                        arrow->setIf(arrowdata->item->getIf());
                        arrow->setUnless(arrowdata->item->getUnless());
                        arrowdata->startNode->addArrow(arrow);
                        targetNode->addArrow(arrow);
                        arrow->setZValue(-20);
                        scene->addItem(arrow);
                        arrow->updatePosition();
                        foundTarget=true;
                    }
                }
            }
        }
    }
    //target not found -> create an included remapitem
    if (!foundTarget){
        QTime time = QTime::currentTime();

        arrowdata->startNode->addRemapItem(arrowdata->item);
        ui->textEdit_Info->append("<font color=\"red\">"+time.toString()+" - warning: <font color=\"blue\">did not find the remap target");
        ui->textEdit_Info->append("<font color=\"black\">  Remapping from <font color=\"red\">"+arrowdata->item->getFrom()+"<font color=\"black\"> to <font color=\"red\">"+arrowdata->item->getTo()+"<font color=\"black\"> could not get allocated!");
        ui->textEdit_Info->append("<font color=\"blue\">  The remap information got added to the from node:<font color=\"red\"> "+arrowdata->startNode->getName());
        ui->dockWidget_errors->show();
    }

    //qDebug()<<foundTarget;

}



/*!\brief <rosparam>-tag object
                         *
                         * get attributes for the object..
                         */
void RxDev::create_rosparamItem(RosparamItem &newRosparam,TiXmlNode *rosparamNode, int &x, int &y)
{
    TiXmlAttribute* tagAttribute=rosparamNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="command"){
            newRosparam.setType(QString("command "+QString(tagAttribute->Value())));
        }else if (QString(tagAttribute->Name())=="file"){
            newRosparam.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="param"){
            newRosparam.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ns"){
            newRosparam.setNamespace(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="if"){
            newRosparam.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newRosparam.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }
    TiXmlNode * pChild;
    for ( pChild = rosparamNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;
    }
    if (rosparamNode->ToElement()->GetText() !="")                  //check for yaml-text
        newRosparam.setText(rosparamNode->ToElement()->GetText());
    newRosparam.updateRosparamItem();
}

/*!\brief <env>-tag object
 *
 * get attributes for the <env>-object.
 */
void RxDev::create_envItem(EnvItem &newEnv,TiXmlNode *envNode, int &x, int &y)
{
    TiXmlAttribute* tagAttribute=envNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="name"){
            newEnv.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="value"){
            newEnv.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="if"){
            newEnv.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newEnv.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }
    TiXmlNode * pChild;
    for ( pChild = envNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;
    }

    newEnv.updateEnvItem();
}

/*!\brief <arg>-tag object
                         *
                         * get attributes for the <arg>-object and search for including tags.
                         */
void RxDev::create_argItem(ArgItem &newArg, TiXmlNode *argNode,int &x, int &y)
{
    TiXmlAttribute* tagAttribute=argNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="name"){
            newArg.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="default"){
            newArg.setValue_or_default(2);
            newArg.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="value"){
            newArg.setValue_or_default(1);
            newArg.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="if"){
            newArg.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newArg.setUnless(QString(tagAttribute->Value()));
        }

        tagAttribute=tagAttribute->Next();
    }
    TiXmlNode * pChild;
    for ( pChild = argNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;
    }
    newArg.updateArgItem();
}
/*!\brief <include>-tag object
                         *
                         * get attributes for the <include>-object and search for including tags.
                         */
void RxDev::create_includeFileItem(IncludeFileItem &newIncludeFile, TiXmlNode *includeNode,int &x, int &y)
{
    TiXmlAttribute* tagAttribute=includeNode->ToElement()->FirstAttribute();
    x=0,y=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="file"){
            newIncludeFile.setFile(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ns"){
            newIncludeFile.setNamespace(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="clear_params"){
            if (QString(tagAttribute->Value())=="true")
                newIncludeFile.setClearParams(1);
            else if (QString(tagAttribute->Value())=="false")
                newIncludeFile.setClearParams(2);
            else
                newIncludeFile.setClearParams(0);
        }
        else if (QString(tagAttribute->Name())=="if"){
            newIncludeFile.setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newIncludeFile.setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }
    int includeX,includeY;      //needed as fakepositions

    newIncludeFile.updateIncludeFileItem();
    //check for env and arg items
    TiXmlNode * pChild;
    for ( pChild = includeNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild,includeX,includeY);
            newIncludeFile.addEnvItem(newEnv);

        }else if (QString(pChild->Value())=="arg"){
            ArgItem* newArg = new ArgItem;
            create_argItem(*newArg,pChild,includeX,includeY);
            newIncludeFile.addArgItem(newArg);

        } else if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            x=coords.x();
            y=coords.y();
        }
        //qDebug()<<"x:"<<x<<" y:"<<y;

    }

}

/*!\brief <group>-tag object
                         *
                         * get attributes for the <group>-object and search for children of the <group>-tag.
                         */
void RxDev::create_groupItem(TiXmlNode *groupNode,GroupItem* newGroup)
{
    TiXmlAttribute* tagAttribute=groupNode->ToElement()->FirstAttribute();
    int x=0,y=0;
    int xGroup=0,yGroup=0;
    int width=200,height=200;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="ns"){
            newGroup->setNamespace(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="clear_params"){
            if (QString(tagAttribute->Value())=="true")
                newGroup->setClear_params(1);
            else if (QString(tagAttribute->Value())=="false")
                newGroup->setClear_params(2);
            else
                newGroup->setClear_params(0);
        }else if (QString(tagAttribute->Name())=="if"){
            newGroup->setIf(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="unless"){
            newGroup->setUnless(QString(tagAttribute->Value()));
        }
        tagAttribute=tagAttribute->Next();
    }
    newGroup->updateGroupItem();

    groups.append(newGroup->getNamespace());
    scene->addItem(newGroup);

    //check for included items
    TiXmlNode * pChild;
    for ( pChild = groupNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {

        if (QString(pChild->Value())=="env"){
            int x,y;
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild,x,y);
            newEnv->setParentItem(newGroup);
            scene->addItem(newEnv);

            if (x==0 &&y==0){
                newEnv->setPos(((newEnv->mapToParent(QPoint(x,y)))));
                newEnv->setLocation(newEnv->mapToParent(newEnv->pos()));
            } else{
                newEnv->setPos(QPoint(x,y));
                newEnv->setLocation(newEnv->mapToParent(newEnv->pos()));
            }
        }else if (QString(pChild->Value())=="param"){
            int x,y;
            ParameterItem* newParam = new ParameterItem;
            create_paramItem(*newParam,pChild,x,y);
            newParam->setParentItem(newGroup);
            scene->addItem(newParam);
            if (x==0 &&y==0){
                newParam->setPos(((newParam->mapToParent(QPoint(x,y)))));
                newParam->setLocation(newParam->mapToParent(newParam->pos()));
            } else{
                newParam->setPos(QPoint(x,y));
                newParam->setLocation(newParam->mapToParent(newParam->pos()));

            }
        }else if (QString(pChild->Value())=="rosparam"){
            int x,y;
            RosparamItem* newRosparam = new RosparamItem;
            create_rosparamItem(*newRosparam,pChild,x,y);
            newRosparam->setParentItem(newGroup);
            scene->addItem(newRosparam);
            if (x==0 &&y==0){
                newRosparam->setPos(((newRosparam->mapToParent(QPoint(x,y)))));
                newRosparam->setLocation(newRosparam->mapToParent(newRosparam->pos()));
            } else{
                newRosparam->setPos(QPoint(x,y));
                newRosparam->setLocation(newRosparam->mapToParent(newRosparam->pos()));
            }
        }else if (QString(pChild->Value())=="remap"){
            int x,y;
            RemapItem* newRemap = new RemapItem;
            create_remapItem(*newRemap,pChild,x,y);
            newRemap->setParentItem(newGroup);
            scene->addItem(newRemap);
            if (x==0 &&y==0){
                newRemap->setPos((((QPoint(x,y)))));
                newRemap->setLocation(newRemap->mapToParent(newRemap->pos()));
            } else{
                newRemap->setPos(QPoint(x,y));
                newRemap->setLocation(newRemap->mapToParent(newRemap->pos()));
            }

        }else if (QString(pChild->Value())=="arg"){
            int x,y;
            ArgItem* newArg = new ArgItem;
            create_argItem(*newArg,pChild,x,y);
            newArg->setParentItem(newGroup);
            scene->addItem(newArg);
            if (x==0 &&y==0){
                newArg->setPos(((newArg->mapToParent(QPoint(x,y)))));
                newArg->setLocation(newArg->mapToParent(newArg->pos()));
            } else{
                newArg->setPos(QPoint(x,y));
                newArg->setLocation(newArg->mapToParent(newArg->pos()));
            }
        }
        else if (QString(pChild->Value())=="include"){
            int x,y;
            IncludeFileItem* newIncludeFile = new IncludeFileItem;
            create_includeFileItem(*newIncludeFile,pChild,x,y);
            newIncludeFile->setParentItem(newGroup);
            scene->addItem(newIncludeFile);
            if (x==0 &&y==0){
                newIncludeFile->setPos((newIncludeFile->mapToParent(QPoint(x,y))));
                newIncludeFile->setLocation(newIncludeFile->mapToParent(newIncludeFile->pos()));
            } else{
                newIncludeFile->setPos(QPoint(x,y));
                newIncludeFile->setLocation(newIncludeFile->mapToParent(newIncludeFile->pos()));
            }
            connect(newIncludeFile,SIGNAL(expandItem(QString,GroupItem &)),this,SLOT(expandInclude(const QString &,GroupItem &)));


        }else if (QString(pChild->Value())=="machine"){
            int x,y;
            MachineItem* newMachine = new MachineItem;
            create_machineItem(*newMachine,pChild,x,y);
            newMachine->setParentItem(newGroup);
            scene->addItem(newMachine);
            if (x==0 &&y==0){
                newMachine->setPos(((newMachine->mapToParent(QPoint(x,y)))));
                newMachine->setLocation(newMachine->mapToParent(newMachine->pos()));
            } else{
                newMachine->setPos(QPoint(x,y));
                newMachine->setLocation(newMachine->mapToParent(newMachine->pos()));
            }
        }else if (QString(pChild->Value())=="node"){
            NodeItem *newNode;
            int x1=x,y1=y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(pChild, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,0, pChild, x1 , y1);
            newNode->setParentItem(newGroup);
            scene->addItem(newNode);

            if (x==0 &&y==0){
                newNode->setPos(QPoint(x1,y1));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            }else{
                newNode->setPos(QPoint(x1,y1));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            }

        }else if (QString(pChild->Value())=="test"){
            NodeItem *newNode;
            int x1=x,y1=y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(pChild, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,1, pChild,x,y);
            newNode->setParentItem(newGroup);
            scene->addItem(newNode);

            if (x==0 &&y==0){
                newNode->setPos(QPoint(x1,y1));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            }else{
                newNode->setPos(QPoint(x1,y1));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            }
        }else if (pChild->Type()==TiXmlNode::TINYXML_COMMENT){
            QPoint coords =getCoords(pChild);
            xGroup=coords.x();
            yGroup=coords.y();
            getGroupDim(pChild,width,height);

        }
        if (xGroup==0 &&yGroup==0){
            newGroup->setPos(findSpace(QPoint(xGroup,yGroup)));
            newGroup->setLocation(newGroup->pos());
        } else{
            newGroup->setPos(QPoint(xGroup,yGroup));
            newGroup->setLocation(newGroup->mapToParent(newGroup->pos()));
        }
        newGroup->setWidth(width);
        newGroup->setHeight(height);


    }
    //end: check for included items
}
