#include "rxdev.h"
#include "tagItems/argItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/groupItem.h"
#include "tagItems/remapItem.h"


/*!\brief find <launch> tag
 *
 * Parses the given file to find the important <launch>-tag and start the parsing of the tags
 */
void RxDev::loadDocument( TiXmlNode * documentNode)
{
    if ( !documentNode ) return;
    TiXmlNode * pChild;

    for ( pChild = documentNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {

        if (QString(pChild->Value())=="launch"){
            TiXmlNode * pChild2;
            for ( pChild2 = pChild->FirstChild(); pChild2 != 0; pChild2 = pChild2->NextSibling())
            {
                beginParsing(pChild2);
            }
        }
    }
}

/*!\brief First-level file parsing
 *
 * Parses the launch or project file for tags that are children of the <launch>-tag.
 */
void RxDev::beginParsing(TiXmlNode *firstLevelNode){

    switch ( firstLevelNode->Type() )
    {
    case TiXmlNode::TINYXML_ELEMENT:                        // case with the important <tags>

        if (QString(firstLevelNode->Value())=="machine"){
            MachineItem* newMachine = new MachineItem;
            create_machineItem(*newMachine,firstLevelNode);
            scene->addItem(newMachine);
        }else if (QString(firstLevelNode->Value())=="group"){
            create_groupItem(firstLevelNode);
        }else if (QString(firstLevelNode->Value())=="node"){
            NodeItem *newNode;
            int x,y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(firstLevelNode, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs, x, y);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,0, firstLevelNode);
            scene->addItem(newNode);
            if (project){ //if getprojectfile
                newNode->setPos(QPoint(x,y));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            } else{
                newNode->setPos(findSpace(QPoint(x,y)));
                newNode->setLocation(newNode->pos());
            }
        }else if (QString(firstLevelNode->Value())=="test"){
            NodeItem *newNode;
            int x,y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(firstLevelNode, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs, x, y);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,1, firstLevelNode);
            scene->addItem(newNode);
            if (project){ //if getprojectfile
                newNode->setPos(QPoint(x,y));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            } else{
                newNode->setPos(findSpace(QPoint(x,y)));
                newNode->setLocation(newNode->pos());
            }
        }else if (QString(firstLevelNode->Value())=="param"){
            ParameterItem * newParam = new ParameterItem;
            create_paramItem(*newParam,firstLevelNode);
            scene->addItem(newParam);
        }else if (QString(firstLevelNode->Value())=="rosparam"){
            ParameterItem * newParam = new ParameterItem;
            create_rosparamItem(*newParam,firstLevelNode);
            scene->addItem(newParam);
        }else if (QString(firstLevelNode->Value())=="remap"){
            RemapItem* newRemap = new RemapItem;
            create_remapItem(*newRemap,firstLevelNode);
            scene->addItem(newRemap);
        }else if (QString(firstLevelNode->Value())=="include"){
            IncludeFileItem* newIncludeFile = new IncludeFileItem;
            create_includeFileItem(*newIncludeFile,firstLevelNode);
            scene->addItem(newIncludeFile);
        }else if (QString(firstLevelNode->Value())=="env"){
            EnvItem * newEnv = new EnvItem;
            create_envItem(*newEnv,firstLevelNode);
            scene->addItem(newEnv);
        }else if (QString(firstLevelNode->Value())=="arg"){
            ArgItem * newArg = new ArgItem;
            create_argItem(*newArg,firstLevelNode);
            scene->addItem(newArg);
        }
        break;

    case TiXmlNode::TINYXML_COMMENT:                        // ignored <tag>
        printf( "Comment: \"%s\"", firstLevelNode->Value());
        break;

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

/*!\brief <machine>-tag object
 *
 * get attributes for the <machine>-object and search for including tags.
 */
void RxDev::create_machineItem(MachineItem &newMachine, TiXmlNode *machineNode)
{
    TiXmlAttribute* tagAttribute=machineNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
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
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }
    if (project){
        newMachine.setPos(QPoint(x,y));
        newMachine.setLocation(newMachine.mapToParent(newMachine.pos()));
    } else{
        newMachine.setPos((findSpace(newMachine.mapToParent(QPoint(x,y)))));
        newMachine.setLocation(newMachine.mapToParent(newMachine.pos()));
    }
    newMachine.updateMachineItem();

    //check for env-items
    TiXmlNode * pChild;
    for ( pChild = machineNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        printf( "child" );
        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild);
            newMachine.addEnvItem(newEnv);
        }
    }
}

/*!\brief collect information for <node>- or <test>-tag object
 *
 * collects some information that are needed to create the node or test object. After that it creates the object.
 */
void RxDev::prepare_nodeOrTest(TiXmlNode *nodeNode,
                               QString &nodePackage, QString &nodeType,QStringList &nodeSubs,
                               QStringList &nodePubs,QStringList &nodeArgs,QStringList &nodeSrvs, int &x, int &y){

    TiXmlAttribute* tagAttribute=nodeNode->ToElement()->FirstAttribute();


    int ival=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="type"){
            nodeType =QString(tagAttribute->Value());
        }else if (QString(tagAttribute->Name())=="pkg"){
            nodePackage =QString(tagAttribute->Value());
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }

    //begin: fetch data, if allready a specfile for the node exists
    QList<QStandardItem *> items = model_availableNodes->findItems(nodeType,Qt::MatchExactly, 0);
    QStandardItem *item;
    for (int i = 0; i < items.size(); i++) {
        item = items.at(i);
        if (item->child(1,0)->child(0,0)->text() ==nodePackage){
            nodeParser(item->child(0,0)->child(0,0)->data(Qt::DisplayRole).toString());
            nodeSubs = node.nodeInput;
            nodePubs = node.nodeOutput;
            nodeSrvs = node.nodeServices;
            nodeArgs = node.nodeArguments;
            break;
        }
    }
    //end: fetch data, if allready a specfile for the node exists

}

/*!\brief <node>- or <test>-tag object
 *
 * get attributes for the object and search for including tags.
 */
void RxDev::create_nodeorTestItem(NodeItem &newNode, int nodeOrTest, TiXmlNode *nodeOrTestNode)
{
    TiXmlAttribute* tagAttribute=nodeOrTestNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
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
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }

    newNode.updateNodeItem();

    //check for env, param and rosparam items
    TiXmlNode * pChild;
    for ( pChild = nodeOrTestNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild);
            newNode.addEnvItem(newEnv);

        }else if (QString(pChild->Value())=="param"){
            ParameterItem* newParam = new ParameterItem;
            create_paramItem(*newParam,pChild);
            newNode.addParamItem(newParam);
        }else if (QString(pChild->Value())=="rosparam"){
            ParameterItem* newParam = new ParameterItem;
            create_rosparamItem(*newParam,pChild);
            newNode.addParamItem(newParam);
        }else if (QString(pChild->Value())=="remap"){
            RemapItem* newRemap = new RemapItem;
            create_remapItem(*newRemap,pChild);
            newNode.addRemapItem(newRemap);

        }
    }
}

/*!\brief <parameter>-tag object
 *
 * get attributes for the <parameter>-object.
 */
void RxDev::create_paramItem(ParameterItem &newParam, TiXmlNode *parameterNode)
{
    TiXmlAttribute* tagAttribute=parameterNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
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
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }

    if (project){ //if getprojectfile
        newParam.setPos(QPoint(x,y));
        newParam.setLocation(newParam.mapToParent(newParam.pos()));
    } else{

        newParam.setPos(findSpace(QPoint(x,y)));
        newParam.setLocation(newParam.pos());
    }
    newParam.updateParameterItem();
}

/*!\brief <remap>-tag object
 *
 * get attributes for the object.
 */
void RxDev::create_remapItem(RemapItem &newRemap,TiXmlNode *remapNode)
{
    TiXmlAttribute* tagAttribute=remapNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="from"){
            newRemap.setFrom(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="to"){
            newRemap.setTo(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }
    if (project){ //if getprojectfile
        newRemap.setPos(QPoint(x,y));
        newRemap.setLocation(newRemap.mapToParent(newRemap.pos()));
    } else{
        newRemap.setPos(findSpace(QPoint(x,y)));
        newRemap.setLocation(newRemap.pos());
    }

    newRemap.updateRemapItem();
}

/*!\brief <rosparam>-tag object
 *
 * get attributes for the object..
 */
void RxDev::create_rosparamItem(ParameterItem &newParam,TiXmlNode *rosparamNode)
{
    TiXmlAttribute* tagAttribute=rosparamNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
    while (tagAttribute)
    {
        newParam.setStandardParameter(3);
        if (QString(tagAttribute->Name())=="command"){
            newParam.setType(QString("command "+QString(tagAttribute->Value())));
        }else if (QString(tagAttribute->Name())=="file"){
            newParam.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="param"){
            newParam.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ns"){
            newParam.setNamespace(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }
    if (project){ //if getprojectfile
        newParam.setPos(QPoint(x,y));
        newParam.setLocation(newParam.mapToParent(newParam.pos()));
    } else{
        newParam.setPos(findSpace(QPoint(x,y)));
        newParam.setLocation(newParam.pos());
    }
    newParam.updateParameterItem();
}

/*!\brief <env>-tag object
 *
 * get attributes for the <env>-object.
 */
void RxDev::create_envItem(EnvItem &newEnv,TiXmlNode *envNode)
{
    TiXmlAttribute* tagAttribute=envNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="name"){
            newEnv.setName(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="value"){
            newEnv.setValue(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }

        tagAttribute=tagAttribute->Next();
    }
    if (project){ //if getprojectfile
        newEnv.setPos(QPoint(x,y));
        newEnv.setLocation(newEnv.mapToParent(newEnv.pos()));
    } else{
        newEnv.setPos(findSpace(QPoint(x,y)));
        newEnv.setLocation(newEnv.pos());
    }

    newEnv.updateEnvItem();
}

/*!\brief <arg>-tag object
 *
 * get attributes for the <arg>-object and search for including tags.
 */
void RxDev::create_argItem(ArgItem &newArg, TiXmlNode *argNode)
{
    TiXmlAttribute* tagAttribute=argNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
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
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }

        tagAttribute=tagAttribute->Next();
    }
    if (project){ //if getprojectfile
        newArg.setPos(QPoint(x,y));
        newArg.setLocation(newArg.mapToParent(newArg.pos()));
    } else{
        newArg.setPos(findSpace(QPoint(x,y)));
        newArg.setLocation(newArg.pos());
    }

    newArg.updateArgItem();
}
/*!\brief <include>-tag object
 *
 * get attributes for the <include>-object and search for including tags.
 */
void RxDev::create_includeFileItem(IncludeFileItem &newIncludeFile, TiXmlNode *includeNode)
{
    TiXmlAttribute* tagAttribute=includeNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
    while (tagAttribute)
    {
        if (QString(tagAttribute->Name())=="file"){
            newIncludeFile.setFile(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="ns"){
            newIncludeFile.setNamespace(QString(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="clear_params"){
            newIncludeFile.setClearParams(bool(tagAttribute->Value()));
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }
        tagAttribute=tagAttribute->Next();
    }
    if (project){ //if getprojectfile
        newIncludeFile.setPos(QPoint(x,y));
        newIncludeFile.setLocation(newIncludeFile.mapToParent(newIncludeFile.pos()));
    } else{
        newIncludeFile.setPos(findSpace(QPoint(x,y)));
        newIncludeFile.setLocation(newIncludeFile.pos());
    }
    newIncludeFile.updateIncludeFileItem();
    //check for env and arg items
    TiXmlNode * pChild;
    for ( pChild = includeNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild);
            newIncludeFile.addEnvItem(newEnv);

        }else if (QString(pChild->Value())=="arg"){
            ArgItem* newArg = new ArgItem;
            create_argItem(*newArg,pChild);
            newIncludeFile.addArgItem(newArg);

        }
    }

}

/*!\brief <group>-tag object
 *
 * get attributes for the <group>-object and search for children of the <group>-tag.
 */
void RxDev::create_groupItem(TiXmlNode *groupNode)
{
    GroupItem* newGroup;
    newGroup = new GroupItem;
    TiXmlAttribute* tagAttribute=groupNode->ToElement()->FirstAttribute();
    int x=0,y=0,ival=0;
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
        }else if (QString(tagAttribute->Name())=="x"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                x =ival;
        }else if (QString(tagAttribute->Name())=="y"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                y =ival;
        }else if (QString(tagAttribute->Name())=="width"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                width =ival;
        }else if (QString(tagAttribute->Name())=="height"){
            if (tagAttribute->QueryIntValue(&ival)==TIXML_SUCCESS)
                height =ival;
        }
        tagAttribute=tagAttribute->Next();
    }
    newGroup->updateGroupItem();
    if (project){ //if getprojectfile
        newGroup->setPos(QPoint(x,y));
        newGroup->setLocation(newGroup->mapToParent(newGroup->pos()));
        newGroup->setWidth(width);
        newGroup->setHeight(height);
    } else{
        newGroup->setPos(findSpace(QPoint(x,y)));
        newGroup->setLocation(newGroup->pos());
        newGroup->setWidth(width);
        newGroup->setHeight(height);

    }

    scene->addItem(newGroup);

    //check for included items
    TiXmlNode * pChild;
    int itemcountA = 0;
    int itemcountB = 0;
    for ( pChild = groupNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {

        if (QString(pChild->Value())=="env"){
            EnvItem* newEnv = new EnvItem;
            create_envItem(*newEnv,pChild);
            itemcountA++;
            newEnv->setParentItem(newGroup);
            scene->addItem(newEnv);

            if (project){ //if getprojectfile
                newEnv->setPos(QPoint(x,y));
                newEnv->setLocation(newEnv->mapToParent(newEnv->pos()));
            } else{
                newEnv->setPos((findSpace(newEnv->mapToParent(QPoint(x,y)))));
                newEnv->setLocation(newEnv->mapToParent(newEnv->pos()));
            }
        }else if (QString(pChild->Value())=="param"){
            ParameterItem* newParam = new ParameterItem;
            create_paramItem(*newParam,pChild);
            itemcountB++;
            newParam->setParentItem(newGroup);
            scene->addItem(newParam);
            if (project){ //if getprojectfile
                newParam->setPos(QPoint(x,y));
                newParam->setLocation(newParam->mapToParent(newParam->pos()));
            } else{
                newParam->setPos((findSpace(newParam->mapToParent(QPoint(x,y)))));
                newParam->setLocation(newParam->mapToParent(newParam->pos()));

            }
        }else if (QString(pChild->Value())=="rosparam"){
            ParameterItem* newParam = new ParameterItem;
            create_rosparamItem(*newParam,pChild);
            itemcountA++;
            newParam->setParentItem(newGroup);
            scene->addItem(newParam);
            if (project){ //if getprojectfile
                newParam->setPos(QPoint(x,y));
                newParam->setLocation(newParam->mapToParent(newParam->pos()));
            } else{
                newParam->setPos((findSpace(newParam->mapToParent(QPoint(x,y)))));
                newParam->setLocation(newParam->mapToParent(newParam->pos()));
            }
        }else if (QString(pChild->Value())=="remap"){
            RemapItem* newRemap = new RemapItem;
            create_remapItem(*newRemap,pChild);
            itemcountA++;
            newRemap->setParentItem(newGroup);
            scene->addItem(newRemap);
            if (project){ //if getprojectfile
                newRemap->setPos(QPoint(x,y));
                newRemap->setLocation(newRemap->mapToParent(newRemap->pos()));
            } else{
                newRemap->setPos((findSpace(newRemap->mapToParent(QPoint(x,y)))));
                newRemap->setLocation(newRemap->mapToParent(newRemap->pos()));
            }
        }else if (QString(pChild->Value())=="arg"){
            ArgItem* newArg = new ArgItem;
            create_argItem(*newArg,pChild);
            itemcountA++;
            newArg->setParentItem(newGroup);
            scene->addItem(newArg);
            if (project){ //if getprojectfile
                newArg->setPos(QPoint(x,y));
                newArg->setLocation(newArg->mapToParent(newArg->pos()));
            } else{
                newArg->setPos((findSpace(newArg->mapToParent(QPoint(x,y)))));
                newArg->setLocation(newArg->mapToParent(newArg->pos()));
            }
        }
        else if (QString(pChild->Value())=="include"){
            IncludeFileItem* newIncludeFile = new IncludeFileItem;
            create_includeFileItem(*newIncludeFile,pChild);
            itemcountA++;
            newIncludeFile->setParentItem(newGroup);
            scene->addItem(newIncludeFile);
            if (project){ //if getprojectfile
                newIncludeFile->setPos(QPoint(x,y));
                newIncludeFile->setLocation(newIncludeFile->mapToParent(newIncludeFile->pos()));
            } else{
                newIncludeFile->setPos((findSpace(newIncludeFile->mapToParent(QPoint(x,y)))));
                newIncludeFile->setLocation(newIncludeFile->mapToParent(newIncludeFile->pos()));
            }
        }else if (QString(pChild->Value())=="machine"){
            MachineItem* newMachine = new MachineItem;
            create_machineItem(*newMachine,pChild);
            itemcountA++;
            newMachine->setParentItem(newGroup);
            scene->addItem(newMachine);
            if (project){ //if getprojectfile
                newMachine->setPos(QPoint(x,y));
                newMachine->setLocation(newMachine->mapToParent(newMachine->pos()));
            } else{
                newMachine->setPos((findSpace(newMachine->mapToParent(QPoint(x,y)))));
                newMachine->setLocation(newMachine->mapToParent(newMachine->pos()));
            }
        }else if (QString(pChild->Value())=="node"){
            NodeItem *newNode;
            int x1=x,y1=y;
            QString nodePackage, nodeType;
            QStringList nodeSubs, nodePubs, nodeArgs, nodeSrvs;
            prepare_nodeOrTest(pChild, nodePackage, nodeType,
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs, x1, y1);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,0, pChild);
            itemcountB++;
            newNode->setParentItem(newGroup);
            scene->addItem(newNode);

            if (true){ //if getprojectfile
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
                               nodeSubs,nodePubs, nodeArgs, nodeSrvs, x1, y1);
            newNode= new NodeItem(nodeType,nodePackage,nodeSubs,nodePubs,nodeSrvs,nodeArgs);
            create_nodeorTestItem(*newNode,1, pChild);
            itemcountB++;
            newNode->setParentItem(newGroup);
            scene->addItem(newNode);

            if (true){ //if getprojectfile
                newNode->setPos(QPoint(x1,y1));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            }else{
                newNode->setPos(QPoint(x1,y1));
                newNode->setLocation(newNode->mapToParent(newNode->pos()));
            }
        }

        //expand the groupitem depending on the included items count
        if (project){
            if (itemcountA*200 > newGroup->getWidth())
                newGroup->setWidth(newGroup->getWidth()+200);
            if (itemcountB*200 > newGroup->getWidth())
                newGroup->setWidth(newGroup->getWidth()+200);
        }
    }
    //end: check for included items
}
