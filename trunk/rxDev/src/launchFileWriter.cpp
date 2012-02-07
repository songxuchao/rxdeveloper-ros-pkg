#include "rxdev.h"

#include "tagItems/parameterItem.h"
#include "tagItems/rosparamItem.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/argItem.h"
#include "tagItems/envItem.h"
#include "tagItems/groupItem.h"
#include "tagItems/nodeItem.h"
#include "tagItems/remapArrow.h"
#include "tagItems/remapItem.h"
#include "launchFileWriter.h"


/*!\brief create launch document
 *
 * Create the <launch> tag and the children tags in the file.
 */
void LaunchWriter::createDocument(QString file, QList<QGraphicsItem *> &list, QString launch_deprecated)
{
    //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    //doc.LinkEndChild( decl );
    TiXmlDocument doc;

    TiXmlElement * launchTag = new TiXmlElement( "launch" );
    if (launch_deprecated !="")
        launchTag->SetAttribute("deprecated", launch_deprecated.toStdString());

    doc.LinkEndChild( launchTag );

    create_commentTag(*launchTag,"Created with rxDev");

    //create tags for the scene items
    QList<int> paramItems;          //List of parameterItems
    QList<int> machineItems;        //List of machineItems
    for (int i = list.size()-1; i > 0; i--) {
        switch (list.at(i)->type()){
        case MachineItem::Type:
                machineItems<<i;                  //save machineItems for later
            break;
        case ParameterItem::Type:
            paramItems<<i;                  //save parameteritems for later
            break;
        default:
            break;
        }
    }
    for (int i=0;i<machineItems.size();i++){
        if (list.at(machineItems.at(i))->parentItem()==0)
            create_machineTag(*launchTag,*list.at(machineItems.at(i)));
    }

    for (int i = list.size()-1; i > 0; i--) {
        switch (list.at(i)->type()){
        case NodeItem::Type:
            if (list.at(i)->parentItem()==0)
                create_nodeTag(*launchTag,*list.at(i));
            break;
        case RemapItem::Type:
            if (list.at(i)->parentItem()==0)
                create_remapTag(*launchTag,*list.at(i));
            break;
        case GroupItem::Type:
            if (list.at(i)->parentItem()==0)
                create_groupTag(*launchTag,*list.at(i));
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
        case RosparamItem::Type:
            if (list.at(i)->parentItem()==0)
                create_rosparamTag(*launchTag,*list.at(i));
            break;
        default:
            break;
        }
    }
    //write parameter tags after everything else
    for (int i=0;i<paramItems.size();i++){
        if (list.at(paramItems.at(i))->parentItem()==0)
            create_paramTag(*launchTag,*list.at(paramItems.at(i)));
    }

    std::string utf8_text = file.toUtf8().constData();
    doc.SaveFile( utf8_text );

}

/*!\brief create <comment> tag
 *
 * Creates a short comment tag.
 */
void LaunchWriter::create_commentTag(TiXmlElement &elem, QString commentText )
{
    TiXmlComment * comment = new TiXmlComment(commentText.toAscii().data());
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
    if (group->getIf()!="")
        groupTag->SetAttribute("if", group->getIf().toStdString());
    if (group->getUnless()!="")
        groupTag->SetAttribute("unless", group->getUnless().toStdString());
    //end optional


    QString xCor,yCor,width,height;
    xCor = QString::number(group->pos().x());
    yCor = QString::number(group->pos().y());
    width = QString::number(group->getWidth());
    height = QString::number(group->getHeight());
    create_commentTag(*groupTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\"").append(" width=\"").append(width).append("\" height=\"").append(height).append("\""));

    //Check for all tags
    QList<QGraphicsItem *> list;
    list=group->childItems();
    QList<int> paramItems;
    for (int i = 0; i < list.size(); i++) {
        switch (list.at(i)->type()){

        case NodeItem::Type:
            create_nodeTag(*groupTag,*list.at(i));
            break;
        case GroupItem::Type:
            create_groupTag(*groupTag,*list.at(i));
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
        case RosparamItem::Type:
            create_rosparamTag(*groupTag,*list.at(i));
            break;
        case ParameterItem::Type:
            paramItems<<i;
            break;
        default:
            break;
        }
    }
    //write parameter tags after everything else
    for (int i=0;i<paramItems.size();i++){
        create_paramTag(*groupTag,*list.at(paramItems.at(i)));
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
    machineTag->SetAttribute("address", machine->getAddress().toStdString());
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
    if (machine->getIf()!="")
        machineTag->SetAttribute("if", machine->getIf().toStdString());
    if (machine->getUnless()!="")
        machineTag->SetAttribute("unless", machine->getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(machine->pos().x());
    yCor = QString::number(machine->pos().y());
    create_commentTag(*machineTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

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
        if (include->getClearParams()==1)  //can only be used if a ns exists
            includeTag->SetAttribute("clear_params", "true");
        else if (include->getClearParams()==2)  //can only be used if a ns exists
            includeTag->SetAttribute("clear_params", "false");
        if (include->getIf()!="")
            includeTag->SetAttribute("if", include->getIf().toStdString());
        if (include->getUnless()!="")
            includeTag->SetAttribute("unless", include->getUnless().toStdString());

    }
    //end optional

    QString xCor,yCor;
    xCor = QString::number(include->pos().x());
    yCor = QString::number(include->pos().y());
    create_commentTag(*includeTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));


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

    }else{ //its a test node
        nodeTag = new TiXmlElement( "test" );
        nodeTag->SetAttribute("test-name", node->getName().toStdString());
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
    if (node->getIf()!="")
        nodeTag->SetAttribute("if", node->getIf().toStdString());
    if (node->getUnless()!="")
        nodeTag->SetAttribute("unless", node->getUnless().toStdString());
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

    if (node->getTime_limit() != "")
        nodeTag->SetAttribute("time-limit", node->getTime_limit().toStdString());
    if (node->getRetry() != "")
        nodeTag->SetAttribute("retry", node->getRetry().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(node->pos().x());
    yCor = QString::number(node->pos().y());
    create_commentTag(*nodeTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

    //Check for embedded remapArrows
    for (int i = 0; i < node->arrows.size(); i++) {
        if (node->arrows.at(i)->startItem()==node)
            create_remapArrowTag(*nodeTag,*node->arrows.at(i));
    }
    //Check for embedded env tags
    for (int i = 0; i < node->envItems.size(); i++) {
        create_envTag(*nodeTag,*node->envItems.at(i));
    }
    //Check for embedded remap tags
    for (int i = 0; i < node->remapItems.size(); i++) {
        create_remapTag(*nodeTag,*node->remapItems.at(i));
    }
    //Check for embedded rosparam tags
    for (int i = 0; i < node->rosparamItems.size(); i++) {
        create_rosparamTag(*nodeTag,*node->rosparamItems.at(i));
    }
    //Check for embedded param tags
    for (int i = 0; i < node->paramItems.size(); i++) {
        create_paramTag(*nodeTag,*node->paramItems.at(i));
    }


    elem.LinkEndChild( nodeTag);

}

/*!\brief create <param> tag
 *
 * Create the <param> tag and sets the attributes.
 */
void LaunchWriter::create_paramTag(TiXmlElement &elem, QGraphicsItem &item )
{
    TiXmlElement * paramTag;
    ParameterItem *parameter = qgraphicsitem_cast<ParameterItem *>(&item);
    paramTag = new TiXmlElement( "param" );
    paramTag->SetAttribute("name", parameter->getName().toStdString());
    if (parameter->getStandardParameter()==1){

        //optional
        if (parameter->getValue()!="")
            paramTag->SetAttribute("value", parameter->getValue().toStdString());
        if (parameter->getType()!="")
            paramTag->SetAttribute("type", parameter->getType().toStdString());
        //end optional
    } else {
        //optional
        if (parameter->getType()=="command")
            paramTag->SetAttribute("command", parameter->getValue().toStdString());
        else if (parameter->getType()=="textfile")
            paramTag->SetAttribute("textfile", parameter->getValue().toStdString());
        else if (parameter->getType()=="binfile")
            paramTag->SetAttribute("binfile", parameter->getValue().toStdString());
        //end optional

    }
    //optional
    if (parameter->getIf()!="")
        paramTag->SetAttribute("if", parameter->getIf().toStdString());
    if (parameter->getUnless()!="")
        paramTag->SetAttribute("unless", parameter->getUnless().toStdString());
    //end optional
    QString xCor,yCor;
    xCor = QString::number(parameter->pos().x());
    yCor = QString::number(parameter->pos().y());
    create_commentTag(*paramTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

    elem.LinkEndChild( paramTag );
}

/*!\brief create <param> tag
 *
 * Create the <param> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_paramTag(TiXmlElement &elem, ParameterItem &parameter )
{
    TiXmlElement * paramTag;
    paramTag = new TiXmlElement( "param" );
    paramTag->SetAttribute("name", parameter.getName().toStdString());
    if (parameter.getStandardParameter()==1){
        //optional
        if (parameter.getValue()!="")
            paramTag->SetAttribute("value", parameter.getValue().toStdString());
        if (parameter.getType()!="")
            paramTag->SetAttribute("type", parameter.getType().toStdString());
        //end optional
    } else{
        //optional
        if (parameter.getType()=="command")
            paramTag->SetAttribute("command", parameter.getValue().toStdString());
        else if (parameter.getType()=="textfile")
            paramTag->SetAttribute("textfile", parameter.getValue().toStdString());
        else if (parameter.getType()=="binfile")
            paramTag->SetAttribute("binfile", parameter.getValue().toStdString());
        //end optional
    }
    //optional
    if (parameter.getIf()!="")
        paramTag->SetAttribute("if", parameter.getIf().toStdString());
    if (parameter.getUnless()!="")
        paramTag->SetAttribute("unless", parameter.getUnless().toStdString());
    //end optional
    QString xCor,yCor;
    xCor = QString::number(parameter.pos().x());
    yCor = QString::number(parameter.pos().y());
    //create_commentTag(*paramTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

    elem.LinkEndChild( paramTag );
}
/*!\brief create <rosparam> tag
 *
 * Create the <rosparam> tag and sets the attributes.
 */
void LaunchWriter::create_rosparamTag(TiXmlElement &elem, QGraphicsItem &item )
{
    TiXmlElement * rosparamTag;
    RosparamItem *rosparam = qgraphicsitem_cast<RosparamItem *>(&item);
    rosparamTag = new TiXmlElement( "rosparam" );
    if (rosparam->getType() == "command load"){
        rosparamTag->SetAttribute("command", "load");
        rosparamTag->SetAttribute("file", rosparam->getValue().toStdString());
    }
    if (rosparam->getType() == "command dump"){
        rosparamTag->SetAttribute("command", "dump");
        rosparamTag->SetAttribute("file", rosparam->getValue().toStdString());
    }
    if (rosparam->getType() == "command delete"){
        rosparamTag->SetAttribute("command", "delete");
        rosparamTag->SetAttribute("param", rosparam->getName().toStdString());
    }
    if (rosparam->getText()!="")
        rosparamTag->LinkEndChild(new TiXmlText(rosparam->getText().toStdString()));
    if (rosparam->getName()!="")
        rosparamTag->SetAttribute("param", rosparam->getName().toStdString());

    //optional
    if (rosparam->getName()!="")
        rosparamTag->SetAttribute("param", rosparam->getName().toStdString());
    if (rosparam->getNamespace()!="")
        rosparamTag->SetAttribute("ns", rosparam->getNamespace().toStdString());

    if (rosparam->getIf()!="")
        rosparamTag->SetAttribute("if", rosparam->getIf().toStdString());
    if (rosparam->getUnless()!="")
        rosparamTag->SetAttribute("unless", rosparam->getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(rosparam->pos().x());
    yCor = QString::number(rosparam->pos().y());
    create_commentTag(*rosparamTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

    elem.LinkEndChild( rosparamTag );
}

/*!\brief create <rosparam> tag
 *
 * Create the<rosparam> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_rosparamTag(TiXmlElement &elem, RosparamItem &rosparam )
{
    TiXmlElement * rosparamTag;

    rosparamTag = new TiXmlElement( "rosparam" );
    rosparamTag->SetAttribute("file", rosparam.getValue().toStdString());
    if (rosparam.getType() == "command load"){
        rosparamTag->SetAttribute("command", "load");
        rosparamTag->SetAttribute("file", rosparam.getValue().toStdString());
    }
    if (rosparam.getType() == "command dump"){
        rosparamTag->SetAttribute("command", "dump");
        rosparamTag->SetAttribute("file", rosparam.getValue().toStdString());
    }
    if (rosparam.getType() == "command delete"){
        rosparamTag->SetAttribute("command", "delete");
        rosparamTag->SetAttribute("param", rosparam.getName().toStdString());
    }
    if (rosparam.getText()!="")
        rosparamTag->LinkEndChild(new TiXmlText(rosparam.getText().toStdString()));
    if (rosparam.getName()!="")
        rosparamTag->SetAttribute("param", rosparam.getName().toStdString());
    //optional
    if (rosparam.getName()!="")
        rosparamTag->SetAttribute("param", rosparam.getName().toStdString());
    if (rosparam.getNamespace()!="")
        rosparamTag->SetAttribute("ns", rosparam.getNamespace().toStdString());

    if (rosparam.getIf()!="")
        rosparamTag->SetAttribute("if", rosparam.getIf().toStdString());
    if (rosparam.getUnless()!="")
        rosparamTag->SetAttribute("unless", rosparam.getUnless().toStdString());
    //end optional
    QString xCor,yCor;
    xCor = QString::number(rosparam.pos().x());
    yCor = QString::number(rosparam.pos().y());
    //create_commentTag(*rosparamTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));


    elem.LinkEndChild( rosparamTag );
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
    //optional
    if (env->getIf()!="")
        envTag->SetAttribute("if", env->getIf().toStdString());
    if (env->getUnless()!="")
        envTag->SetAttribute("unless", env->getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(env->pos().x());
    yCor = QString::number(env->pos().y());
    create_commentTag(*envTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

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
    //optional
    if (env.getIf()!="")
        envTag->SetAttribute("if", env.getIf().toStdString());
    if (env.getUnless()!="")
        envTag->SetAttribute("unless", env.getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(env.pos().x());
    yCor = QString::number(env.pos().y());
    //create_commentTag(*envTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

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
    if (arg.getIf()!="")
        argTag->SetAttribute("if", arg.getIf().toStdString());
    if (arg.getUnless()!="")
        argTag->SetAttribute("unless", arg.getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(arg.pos().x());
    yCor = QString::number(arg.pos().y());
    create_commentTag(*argTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

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
    if (arg->getIf()!="")
        argTag->SetAttribute("if", arg->getIf().toStdString());
    if (arg->getUnless()!="")
        argTag->SetAttribute("unless", arg->getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(arg->pos().x());
    yCor = QString::number(arg->pos().y());
    create_commentTag(*argTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

    elem.LinkEndChild( argTag );
}


/*!\brief create <remap> tag
 *
 * Create the <remap> tag in an other tag and sets the attributes.
 */
void LaunchWriter::create_remapTag(TiXmlElement &elem, RemapItem &remap)
{
    TiXmlElement * remapTag = new TiXmlElement( "remap" );
    remapTag->SetAttribute("from", remap.getFrom().toStdString());
    remapTag->SetAttribute("to", remap.getTo().toStdString());
    //optional
    if (remap.getIf()!="")
        remapTag->SetAttribute("if", remap.getIf().toStdString());
    if (remap.getUnless()!="")
        remapTag->SetAttribute("unless", remap.getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(remap.pos().x());
    yCor = QString::number(remap.pos().y());

    elem.LinkEndChild( remapTag );
}

/*!\brief create <remap> tag
 *
 * Create the <remap> tag and sets the attributes.
 */
void LaunchWriter::create_remapTag(TiXmlElement &elem, QGraphicsItem &item)
{
    TiXmlElement * remapTag = new TiXmlElement( "remap" );
    RemapItem * remap = qgraphicsitem_cast<RemapItem *>(&item);

    remapTag->SetAttribute("from", remap->getFrom().toStdString());
    remapTag->SetAttribute("to", remap->getTo().toStdString());
    //optional
    if (remap->getIf()!="")
        remapTag->SetAttribute("if", remap->getIf().toStdString());
    if (remap->getUnless()!="")
        remapTag->SetAttribute("unless", remap->getUnless().toStdString());
    //end optional

    QString xCor,yCor;
    xCor = QString::number(remap->pos().x());
    yCor = QString::number(remap->pos().y());
    create_commentTag(*remapTag,QString("x=\"").append(xCor).append("\" y=\"").append(yCor).append("\""));

    elem.LinkEndChild( remapTag );
}

void LaunchWriter::create_remapArrowTag(TiXmlElement &nodeTag, RemapArrow &arrow)
{
    TiXmlElement * remapTag = new TiXmlElement( "remap" );
    remapTag->SetAttribute("from", arrow.getFrom().toStdString());
    remapTag->SetAttribute("to", arrow.getTo().toStdString());
    //optional
    if (arrow.getIf()!="")
        remapTag->SetAttribute("if", arrow.getIf().toStdString());
    if (arrow.getUnless()!="")
        remapTag->SetAttribute("unless", arrow.getUnless().toStdString());
    //end optional

    nodeTag.LinkEndChild( remapTag );
}
