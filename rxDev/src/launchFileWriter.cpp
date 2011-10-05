#include "rxdev.h"

#include "tagItems/parameterItem.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/argItem.h"
#include "tagItems/envItem.h"
#include "tagItems/groupItem.h"
#include "tagItems/nodeItem.h"
#include "tagItems/remapArrow.h"
#include "tagItems/remapItem.h"
#include "launchFileWriter.h"


/*!\brief create project or launch document
 *
 * Create the <launch> tag and the children tags in the file.
 */
void LaunchWriter::createDocument(QString file, bool _project, QList<QGraphicsItem *> &list)
{
    //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    //doc.LinkEndChild( decl );
    TiXmlDocument doc;
    project=_project;
    TiXmlElement * launchTag = new TiXmlElement( "launch" );
    doc.LinkEndChild( launchTag );

    create_commentTag(*launchTag);

    //create tags for the scene items
    for (int i = 0; i < list.size(); i++) {
        switch (list.at(i)->type()){

        case NodeItem::Type:
            if (list.at(i)->parentItem()==0)
                create_nodeTag(*launchTag,*list.at(i));
            break;
        case GroupItem::Type:
            if (list.at(i)->parentItem()==0)
                create_groupTag(*launchTag,*list.at(i));

            break;
        case ParameterItem::Type:
            if (list.at(i)->parentItem()==0)
                create_paramTag(*launchTag,*list.at(i));
            break;
        case MachineItem::Type:
            if (list.at(i)->parentItem()==0)
                create_machineTag(*launchTag,*list.at(i));
            break;
        case EnvItem::Type:
            if (list.at(i)->parentItem()==0)
                create_envTag(*launchTag,*list.at(i));
            break;
        case ArgItem::Type:
            if (list.at(i)->parentItem()==0)
                create_argTag(*launchTag,*list.at(i));
            break;
        case IncludeFileItem::Type:
            if (list.at(i)->parentItem()==0)
                create_includeTag(*launchTag,*list.at(i));
            break;
        case RemapItem::Type:
            if (list.at(i)->parentItem()==0)
                create_remapTag(*launchTag,*list.at(i));
            break;
        default:
            break;
        }
    }


    doc.SaveFile( file.toStdString() );

}

/*!\brief create <comment> tag
 *
 * Creates a short comment tag.
 */
void LaunchWriter::create_commentTag(TiXmlElement &elem)
{
    TiXmlComment * comment = new TiXmlComment("Created with rxDev");
    elem.LinkEndChild( comment );
}

/*!\brief create <group> tag
 *
 * Create the <group> tag, sets the attributes and finds included tags.
 */
void LaunchWriter::create_groupTag(TiXmlElement &elem, QGraphicsItem &item)
{
    GroupItem *group = qgraphicsitem_cast<GroupItem *>(&item);
    TiXmlElement * groupTag = new TiXmlElement( "group" );

    //optional
    if (group->getNamespace() != "")
        groupTag->SetAttribute("ns", group->getNamespace().toStdString());
    if (group->getClear_params() == 1)
        groupTag->SetAttribute("clear_params", "true");
    else if (group->getClear_params() == 2)
        groupTag->SetAttribute("clear_params", "false");
    //end optional

    //for projectfiles
    if (project){ //if writing projectfile
        groupTag->SetAttribute("x",group->getLocation().x());
        groupTag->SetAttribute("y",group->getLocation().y());
        groupTag->SetAttribute("width",group->getWidth());
        groupTag->SetAttribute("height",group->getHeight());
    }

    //Check for all tags
    QList<QGraphicsItem *> list;
    list=group->childItems();
    for (int i = 0; i < list.size(); i++) {
        switch (list.at(i)->type()){

        case NodeItem::Type:
            create_nodeTag(*groupTag,*list.at(i));
            break;
        case GroupItem::Type:
            create_groupTag(*groupTag,*list.at(i));
            break;
        case ParameterItem::Type:
            create_paramTag(*groupTag,*list.at(i));
            break;
        case MachineItem::Type:
            create_machineTag(*groupTag,*list.at(i));
            break;
        case EnvItem::Type:
            create_envTag(*groupTag,*list.at(i));
            break;
        case ArgItem::Type:
            create_argTag(*groupTag,*list.at(i));
            break;
        case IncludeFileItem::Type:
            create_includeTag(*groupTag,*list.at(i));
            break;
        case RemapItem::Type:
            create_remapTag(*groupTag,*list.at(i));
            break;
//        case 65550:
//            create_remapTag(*groupTag,*list.at(i));
//            break;
        default:
            break;
        }
    }


    elem.LinkEndChild( groupTag);
}

/*!\brief create <machine> tag
 *
 * Create the <machine> tag, sets the attributes and finds included tags.
 */
void LaunchWriter::create_machineTag(TiXmlElement &elem, QGraphicsItem &item)
{
    TiXmlElement * machineTag = new TiXmlElement( "machine" );
    MachineItem *machine = qgraphicsitem_cast<MachineItem *>(&item);

    machineTag->SetAttribute("name", machine->getName().toStdString());
    machineTag->SetAttribute("adress", machine->getAddress().toStdString());
    //optional
    if (machine->getRos_root()!="")
        machineTag->SetAttribute("ros-root", machine->getRos_root().toStdString());
    if (machine->getRos_package_path()!="")
        machineTag->SetAttribute("ros-package-path", machine->getRos_package_path().toStdString());
    if (machine->getDefault()!="")
        machineTag->SetAttribute("default", machine->getDefault().toStdString());
    if (machine->getUser()!="")
        machineTag->SetAttribute("user", machine->getUser().toStdString());
    if (machine->getPassword()!="")
        machineTag->SetAttribute("password", machine->getPassword().toStdString());
    if (machine->getTimeout()!="")
        machineTag->SetAttribute("timeout", machine->getTimeout().toStdString());
    //end optional
    //for projectfiles
    if (project){ //if writing projectfile
        machineTag->SetAttribute("x",machine->pos().x());
        machineTag->SetAttribute("y",machine->pos().y());
    }

    //Check for embedded env tags
    for (int i = 0; i < machine->envItems.size(); i++) {
        create_envTag(*machineTag,*machine->envItems.at(i));
    }

    elem.LinkEndChild( machineTag );
}

/*!\brief create <include> tag
 *
 * Create the <include> tag, sets the attributes and finds included tags.
 */
void LaunchWriter::create_includeTag(TiXmlElement &elem, QGraphicsItem &item)
{
    TiXmlElement * includeTag = new TiXmlElement( "include" );
    IncludeFileItem *include = qgraphicsitem_cast<IncludeFileItem *>(&item);

    includeTag->SetAttribute("file", include->getFile().toStdString());
    //optional
    if (include->getNamespace()!=""){
        includeTag->SetAttribute("ns", include->getNamespace().toStdString());
        if (include->getClearParams()==true)  //can only be used if a ns exists
            includeTag->SetAttribute("clear_params", "true");
        else if (include->getClearParams()==false)  //can only be used if a ns exists
            includeTag->SetAttribute("clear_params", "false");

    }
    //end optional
    //for projectfiles
    if (project){ //if writing projectfile
        includeTag->SetAttribute("x",include->pos().x());
        includeTag->SetAttribute("y",include->pos().y());
    }

    //Check for embedded env tags
    for (int i = 0; i < include->envItems.size(); i++) {

        create_envTag(*includeTag,*include->envItems.at(i));
    }

    //Check for embedded arg tags
    for (int i = 0; i < include->argItems.size(); i++) {
        create_argTag(*includeTag,*include->argItems.at(i));
    }

    elem.LinkEndChild( includeTag );
}

/*!\brief create <node> tag
 *
 * Create the <node> tag, sets the attributes and finds included tags.
 */
void LaunchWriter::create_nodeTag(TiXmlElement &elem, QGraphicsItem &item)
{
    NodeItem *node = qgraphicsitem_cast<NodeItem *>(&item);
    TiXmlElement * nodeTag;
    if (node->getNode_or_test()==0){
        nodeTag = new TiXmlElement( "node" );
        nodeTag->SetAttribute("name", node->getName().toStdString());
        //optional
        if (node->getMachine() != "")
            nodeTag->SetAttribute("machine", node->getMachine().toStdString());
        if (node->getRespawn() == 1)
            nodeTag->SetAttribute("respawn", "true");
        if (node->getRespawn() == 2)
            nodeTag->SetAttribute("respawn", "false");
        if (node->getOutput() == 1)
            nodeTag->SetAttribute("output", "log");
        else if (node->getOutput() == 2)
            nodeTag->SetAttribute("output", "screen");
        //end optional
    }else{ //its a test node
        nodeTag = new TiXmlElement( "test" );
        //Todo::Check for env remap rosparam and param tags
        nodeTag->SetAttribute("test-name", node->getName().toStdString());
        //optional
        if (node->getTime_limit() != "")
            nodeTag->SetAttribute("time-limit", node->getTime_limit().toStdString());
        if (node->getRetry() != "")
            nodeTag->SetAttribute("retry", node->getRetry().toStdString());
        //end optional
    }
    nodeTag->SetAttribute("pkg", node->getPkg().toStdString());
    nodeTag->SetAttribute("type", node->getType().toStdString());
    //optional
    if (node->getArgs() != "")
        nodeTag->SetAttribute("args", node->getArgs().toStdString());
    if (node->getRequired() == 1)
        nodeTag->SetAttribute("required", "true");
    else if (node->getRequired() == 2)
        nodeTag->SetAttribute("required", "false");
    if (node->getNamespace() != "")
        nodeTag->SetAttribute("ns", node->getNamespace().toStdString());
    if (node->getClear_params() == 1)
        nodeTag->SetAttribute("clear_params", "true");
    else if (node->getClear_params() == 2)
        nodeTag->SetAttribute("clear_params", "false");
    if (node->getCwd() == 1)
        nodeTag->SetAttribute("cwd", "ROS_HOME");
    if (node->getCwd() == 2)
        nodeTag->SetAttribute("cwd", "node");
    if (node->getLaunch_prefix() != "")
        nodeTag->SetAttribute("launch-prefix", node->getLaunch_prefix().toStdString());
    //end optional

    //for projectfiles
    if (project){ //if writing projectfile
        nodeTag->SetAttribute("x",node->getLocation().x());
        nodeTag->SetAttribute("y",node->getLocation().y());
    }

    //Check for embedded env tags
    for (int i = 0; i < node->envItems.size(); i++) {

        create_envTag(*nodeTag,*node->envItems.at(i));
    }
    //Check for embedded param or rosparam tags
    for (int i = 0; i < node->paramItems.size(); i++) {
        create_paramTag(*nodeTag,*node->paramItems.at(i));
    }

    //Check for embedded remap tags
    for (int i = 0; i < node->remapItems.size(); i++) {
        create_remapTag(*nodeTag,*node->remapItems.at(i));
            }

    elem.LinkEndChild( nodeTag);

}

/*!\brief create <param> or <rosparam> tag
 *
 * Create the <param> or <rosparam> tag and sets the attributes.
 */
void LaunchWriter::create_paramTag(TiXmlElement &elem, QGraphicsItem &item )
{
    TiXmlElement * paramTag;
    ParameterItem *parameter = qgraphicsitem_cast<ParameterItem *>(&item);
    if (parameter->getStandardParameter()==1){
        paramTag = new TiXmlElement( "param" );
        paramTag->SetAttribute("name", parameter->getName().toStdString());
        //optional
        if (parameter->getValue()!="")
            paramTag->SetAttribute("value", parameter->getValue().toStdString());
        if (parameter->getType()!="")
            paramTag->SetAttribute("type", parameter->getType().toStdString());
        //end optional

    } else if (parameter->getStandardParameter()==2){
        paramTag = new TiXmlElement( "param" );
        paramTag->SetAttribute("name", parameter->getName().toStdString());
        //optional
        if (parameter->getType()=="command")
            paramTag->SetAttribute("command", parameter->getValue().toStdString());
        else if (parameter->getType()=="textfile")
            paramTag->SetAttribute("textfile", parameter->getValue().toStdString());
        else if (parameter->getType()=="binfile")
            paramTag->SetAttribute("binfile", parameter->getValue().toStdString());
        //end optional

    }else{
        paramTag = new TiXmlElement( "rosparam" );
        if (parameter->getType() == "command load")
            paramTag->SetAttribute("command", "load");
        if (parameter->getType() == "command dump")
            paramTag->SetAttribute("command", "dump");
        if (parameter->getType() == "command delete")
            paramTag->SetAttribute("command", "delete");
//        qDebug()<<"type: "<<parameter->getType();
//        qDebug()<<"ns: "<<parameter->getNamespace();
//        qDebug()<<"name: "<<parameter->getName();
//        qDebug()<<"Value: "<<parameter->getValue();
        paramTag->SetAttribute("file", parameter->getValue().toStdString());
        //optional
        if (!parameter->getName().isEmpty())
            paramTag->SetAttribute("param", parameter->getName().toStdString());
        if (!parameter->getNamespace().isEmpty())
            paramTag->SetAttribute("ns", parameter->getNamespace().toStdString());
        //end optional
    }

    //for projectfiles
    if (project){ //if writing projectfile
        paramTag->SetAttribute("x",parameter->pos().x());
        paramTag->SetAttribute("y",parameter->pos().y());
    }
    elem.LinkEndChild( paramTag );
}

/*!\brief create <param> or <rosparam> tag
 *
 * Create the <param> or <rosparam> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_paramTag(TiXmlElement &elem, ParameterItem &parameter )
{
    TiXmlElement * paramTag;
    if (parameter.getStandardParameter()==1){
        paramTag = new TiXmlElement( "param" );
        paramTag->SetAttribute("name", parameter.getName().toStdString());
        //optional
        if (parameter.getValue()!="")
            paramTag->SetAttribute("value", parameter.getValue().toStdString());
        if (parameter.getType()!="")
            paramTag->SetAttribute("type", parameter.getType().toStdString());
        //end optional

    } else if (parameter.getStandardParameter()==2){
        paramTag = new TiXmlElement( "param" );
        paramTag->SetAttribute("name", parameter.getName().toStdString());
        //optional
        if (parameter.getType()=="command")
            paramTag->SetAttribute("command", parameter.getValue().toStdString());
        else if (parameter.getType()=="textfile")
            paramTag->SetAttribute("textfile", parameter.getValue().toStdString());
        else if (parameter.getType()=="binfile")
            paramTag->SetAttribute("binfile", parameter.getValue().toStdString());
        //end optional

    }else{
        paramTag = new TiXmlElement( "rosparam" );
        if (parameter.getName()!="")
            paramTag->SetAttribute("param", parameter.getName().toStdString());
        if (parameter.getType() == "command load")
            paramTag->SetAttribute("command", "load");
        if (parameter.getType() == "command dump")
            paramTag->SetAttribute("command", "dump");
        if (parameter.getType() == "command delete")
            paramTag->SetAttribute("command", "delete");
        qDebug()<<"type: "<<parameter.getType();
        qDebug()<<"ns: "<<parameter.getNamespace();
        qDebug()<<"name: "<<parameter.getName();
        qDebug()<<"Value: "<<parameter.getValue();
        paramTag->SetAttribute("file", parameter.getValue().toStdString());
        paramTag->SetAttribute("param", parameter.getName().toStdString());
        //optional
        paramTag->SetAttribute("ns", parameter.getNamespace().toStdString());
        //end optional

    }

    //for projectfiles
    if (project){ //if writing projectfile
        paramTag->SetAttribute("x",parameter.pos().x());
        paramTag->SetAttribute("y",parameter.pos().y());
    }

    elem.LinkEndChild( paramTag );
}

/*!\brief create <env> tag
 *
 * Create the <env> tag and sets the attributes.
 */
void LaunchWriter::create_envTag(TiXmlElement &elem, QGraphicsItem &item)
{
    TiXmlElement * envTag = new TiXmlElement( "env" );
    EnvItem * env = qgraphicsitem_cast<EnvItem *>(&item);
    envTag->SetAttribute("name", env->getName().toStdString());
    envTag->SetAttribute("value", env->getValue().toStdString());
    //for projectfiles
    if (project){ //if writing projectfile
        envTag->SetAttribute("x",env->pos().x());
        envTag->SetAttribute("y",env->pos().y());
    }
    elem.LinkEndChild( envTag );
}


/*!\brief create <env> tag
 *
 * Create the <env> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_envTag(TiXmlElement &elem, EnvItem &env)
{
    TiXmlElement * envTag = new TiXmlElement( "env" );
    envTag->SetAttribute("name", env.getName().toStdString());
    envTag->SetAttribute("value", env.getValue().toStdString());
    //for projectfiles
    if (project){ //if writing projectfile
        envTag->SetAttribute("x",env.pos().x());
        envTag->SetAttribute("y",env.pos().y());
    }
    elem.LinkEndChild( envTag );
}


/*!\brief create <arg> tag
 *
 * Create the <arg> tag and sets the attributes.
 */
void LaunchWriter::create_argTag(TiXmlElement &elem, ArgItem &arg)
{
    TiXmlElement * argTag = new TiXmlElement( "arg" );

    argTag->SetAttribute("name", arg.getName().toStdString());
    //optional
    if (arg.getValue_or_default()==1)
        argTag->SetAttribute("value", arg.getValue().toStdString());
    else if (arg.getValue_or_default()==2)
        argTag->SetAttribute("default", arg.getValue().toStdString());
    //end optional
    //for projectfiles
    if (project){ //if writing projectfile
        argTag->SetAttribute("x",arg.pos().x());
        argTag->SetAttribute("y",arg.pos().y());
    }
    elem.LinkEndChild( argTag );
}


/*!\brief create <arg> tag
 *
 * Create the <arg> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_argTag(TiXmlElement &elem, QGraphicsItem &item)
{
    TiXmlElement * argTag = new TiXmlElement( "arg" );
    ArgItem * arg = qgraphicsitem_cast<ArgItem *>(&item);
    argTag->SetAttribute("name", arg->getName().toStdString());
    //optional
    if (arg->getValue_or_default()==1)
        argTag->SetAttribute("value", arg->getValue().toStdString());
    else if (arg->getValue_or_default()==2)
        argTag->SetAttribute("default", arg->getValue().toStdString());
    //end optional
    //for projectfiles
    if (project){ //if writing projectfile
        argTag->SetAttribute("x",arg->pos().x());
        argTag->SetAttribute("y",arg->pos().y());
    }
    elem.LinkEndChild( argTag );
}


///*!\brief create <remap> tag
// *
// * Create the <remap> tag and sets the attributes.
// */
//void LaunchWriter::create_remapTag(TiXmlElement &elem, RemapArrow &arrow)
//{
//    TiXmlElement * remapTag = new TiXmlElement( "remap" );
//    remapTag->SetAttribute("from", arrow.getFrom().toStdString());
//    remapTag->SetAttribute("to", arrow.getTo().toStdString());
//    elem.LinkEndChild( remapTag );
//}

/*!\brief create <remap> tag
 *
 * Create the <remap> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_remapTag(TiXmlElement &elem, QGraphicsItem &item)
{
    TiXmlElement * remapTag = new TiXmlElement( "remap" );
    RemapItem * remap = qgraphicsitem_cast<RemapItem *>(&item);

    remapTag->SetAttribute("from", remap->getFrom().toStdString());
    remapTag->SetAttribute("to", remap->getTo().toStdString());
    //for projectfiles
    if (project){ //if writing projectfile
        remapTag->SetAttribute("x",remap->pos().x());
        remapTag->SetAttribute("y",remap->pos().y());
    }
    elem.LinkEndChild( remapTag );
}
