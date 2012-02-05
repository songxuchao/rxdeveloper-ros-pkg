#include "rxdev.h"
#include "ui_rxdev.h"
#include "tagItems/parameterItem.h"
#include "tagItems/rosparamItem.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/groupItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/argItem.h"
#include "tagItems/envItem.h"
#include "tagItems/remapItem.h"
#include "std_msgs/String.h"
#include "launchFileView.h"
#include "launchFileScene.h"
#include "specFileEdit.h"
#include "specFileParser.h"
#include <yaml-cpp/yaml.h>
#include <QUrl>

//QPoint menuPoint_availableNodes;


void RxDev::setupConnector(){
    ui->actionDrag_Drop->setChecked(true);

    //scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    gview = new LaunchFileView();
    scene = new LaunchFileScene(this,gview->width(), gview->height());
    on_actionDrag_Drop_triggered();

    ui->hLayout->addWidget( gview );
    /*
     setSizePolicy, when used with the horizontalLayout widget, allows the grapicsview to expand/contract
     as the user re-sizes the main window.
    */
    gview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    /*
        add the scene to the QGraphicsView
    */
    gview->setScene( scene);
    gview->show(); //let the show begin
}


void RxDev::availableNodesOrComps() {

    /* Create the data model */

    model_availableNodes  = new QStandardItemModel();
    ui->treeView_availableNodes->setModel(model_availableNodes);
    ui->treeView_availableNodes->setContextMenuPolicy(Qt::CustomContextMenu);

    model_availableComponents = new QStandardItemModel();
    ui->listView_availableComponents->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listView_availableComponents->setModel(model_availableComponents);
    on_pushButton_refreshNodesOrComps_clicked();

    connect(ui->listView_availableComponents, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(addCompFile()));
    connect(ui->listView_availableComponents, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu_availableComponents(const QPoint&)));

    connect(ui->treeView_availableNodes, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu_availableNodes(const QPoint&)));
    connect(ui->treeView_availableNodes->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this, SLOT(selectionHandle_availableNodes(const QItemSelection &,const QItemSelection &)));

}

void RxDev::fillItemModel_availableNodes(QString nodeFile, Specfile &node){

    QBrush b;

    QStandardItem *group = new QStandardItem(QString("%1").arg(QString::fromStdString(node.type)));

    QStandardItem *child;
    QStandardItem *item;
        QStandardItem *item2;
    child = new QStandardItem(QString("path"));
    item = new QStandardItem(QString("%1").arg(nodeFile));
    b.setColor(Qt::blue);
    child->setForeground(b);
    b.setColor(Qt::black);
    item->setForeground(b);
    child->appendRow(item);
    // the appendRow function appends the child as new row
    group->appendRow(child);


    child = new QStandardItem(QString("type"));
    item = new QStandardItem(QString("%1").arg(QString::fromStdString(node.type)));
    child->appendRow(item);
    child = new QStandardItem(QString("package"));
    item = new QStandardItem(QString("%1").arg(QString::fromStdString(node.package)));
    b.setColor(Qt::blue);
    child->setForeground(b);
    child->appendRow(item);
    group->appendRow(child);

    child = new QStandardItem(QString("subscriptions"));
     for(std::list<Topic_Type>::iterator iter=node.subscriptions.begin();iter != node.subscriptions.end();iter++ )
     {
         item = new QStandardItem(QString::fromStdString(Topic_Type(*iter).topic));
         item2 = new QStandardItem(QString::fromStdString(Topic_Type(*iter).topictype));
         b.setColor(Qt::red);
         item->setForeground(b);
         b.setColor(Qt::gray);
         item2->setForeground(b);
         item->appendRow(item2);
         child->appendRow(item);
     }
     b.setColor(Qt::blue);
     child->setForeground(b);


    group->appendRow(child);

    child = new QStandardItem(QString("publications"));
    for(std::list<Topic_Type>::iterator iter=node.publications.begin();iter != node.publications.end();iter++ )
    {
        item = new QStandardItem(QString::fromStdString(Topic_Type(*iter).topic));
        item2 = new QStandardItem(QString::fromStdString(Topic_Type(*iter).topictype));
        b.setColor(Qt::red);
        item->setForeground(b);
        b.setColor(Qt::gray);
        item2->setForeground(b);
        item->appendRow(item2);
        child->appendRow(item);
    }
    b.setColor(Qt::blue);
    child->setForeground(b);


    group->appendRow(child);

    child = new QStandardItem(QString("services"));
    for(std::list<Topic_Type>::iterator iter=node.services.begin();iter != node.services.end();iter++ )
    {
        item = new QStandardItem(QString::fromStdString(Topic_Type(*iter).topic));
        item2 = new QStandardItem(QString::fromStdString(Topic_Type(*iter).topictype));
        b.setColor(Qt::red);
        item->setForeground(b);
        b.setColor(Qt::gray);
        item2->setForeground(b);
        item->appendRow(item2);
        child->appendRow(item);
    }
    b.setColor(Qt::blue);
    child->setForeground(b);

    group->appendRow(child);

    child = new QStandardItem(QString("parameters"));
    for(std::list<Name_Type_Default>::iterator iter=node.parameters.begin();iter != node.parameters.end();iter++ )
    {
        item = new QStandardItem(QString::fromStdString(Name_Type_Default(*iter).paramName));
        item2 = new QStandardItem("type: "+QString::fromStdString(Name_Type_Default(*iter).paramType));
        b.setColor(Qt::red);
        item->setForeground(b);
        b.setColor(Qt::gray);
        item2->setForeground(b);
        item->appendRow(item2);
        item2 = new QStandardItem("default: "+QString::fromStdString(Name_Type_Default(*iter).paramDefault));
        b.setColor(Qt::black);
        item2->setForeground(b);
        item->appendRow(item2);
        item2 = new QStandardItem("range: "+QString::fromStdString(Name_Type_Default(*iter).paramRange));
        b.setColor(Qt::gray);
        item2->setForeground(b);
        item->appendRow(item2);
        child->appendRow(item);
    }
    b.setColor(Qt::blue);
    child->setForeground(b);

    group->appendRow(child);
    b.setColor(Qt::darkBlue);
    QFont font;
    font.setBold(true);
    group->setFont(font);
    group->setForeground(b);

    // append group as new row to the model. model takes the ownership of the item
    if (!(node.type.length()==0) && !(node.package.length()==0))
        model_availableNodes->appendRow(group);
        ui->treeView_availableNodes->setAutoFillBackground(true);
}



void RxDev::on_actionNew_Launchfile_triggered()
{
    QMessageBox::StandardButton button = QMessageBox::warning(this, QString(),
                                                              tr("Did you save your progress?\nDo you really want to create a new File?"),
                                                              QMessageBox::Yes | QMessageBox::Cancel);
    if (button == QMessageBox::Yes){
        QList<QGraphicsItem *> list;
        list=scene->items();
        for (int i = 0; i < list.size(); i++) {
            scene->removeItem(list.at(i));
        }
        setWindowTitle("rxDeveloper - *new");
        gview->setDeprecated(false);
        gview->setMessage("");
    }
}


void RxDev::on_pushButton_refreshNodesOrComps_clicked()
{

    ui->statusBar->showMessage(tr("refreshing nodelist...please wait!"));
    packageList.clear();
    model_availableNodes->clear();
    model_availableComponents->clear();
    //get all available packages
    QProcess findPackages;
    findPackages.start(QString("rospack list-names"));
    findPackages.waitForFinished(-1);
    QByteArray output = findPackages.readAll();
    packageList = QString(output).split("\n");
    packageList.removeDuplicates();
    packageList.sort();
    packageList.removeFirst();

    //for all available packages do
    QStringList availableNodeList;
    QStringList availableCompList;

    for(int pack=0; pack<packageList.count();pack++){
        //Get package-path
        QProcess getPackagePath;
        getPackagePath.setWorkingDirectory(workingDir.absolutePath());
        getPackagePath.start(QString("rospack find "+packageList[pack]));
        getPackagePath.waitForFinished(-1);
        QByteArray output = getPackagePath.readAllStandardOutput();
        QString packagePath = output.trimmed();

        //find .node-files
        QProcess getNodes;
        getNodes.setWorkingDirectory(workingDir.absolutePath());
        QString command = "find -H "+packagePath+"/node";
        getNodes.start(command);
        getNodes.waitForFinished(-1);
        output = getNodes.readAllStandardOutput();
        availableNodeList.append(QString(output).split("\n"));

        //find component-launch-files
        QProcess getComponents;
        getComponents.setWorkingDirectory(workingDir.absolutePath());
        command = "find -H "+packagePath+"/component";
        getComponents.start(command);
        getComponents.waitForFinished(-1);
        output = getComponents.readAllStandardOutput();
        availableCompList.append(QString(output).split("\n"));
    }
    availableNodeList.removeDuplicates();
    availableNodeList.sort();
    availableCompList.removeDuplicates();
    availableCompList.sort();


    //parse all available specfiles
    SpecFileParser *specParse=new SpecFileParser;
    for (int i=0; i<availableNodeList.count();i++){
        if (QString(availableNodeList[i]).endsWith(".node")){
            //qDebug()<<availableNodeList[i];

            //fetch nodespecs
            try{
                specParse->nodeParser(availableNodeList[i]);
            }catch(YAML::InvalidScalar &e){
                qDebug()<<"Invalid scalar in"<<QString(availableNodeList[i]);
                qDebug()<<e.what();
                //break;
            }catch (YAML::ParserException&e){
                qDebug()<<"Parser exception in"<<QString(availableNodeList[i]);
                qDebug()<<e.what();
                //break;
            } catch (YAML::BadDereference&e){
                qDebug()<<"Bad Dereference in"<<QString(availableNodeList[i]);
                qDebug()<<e.what();
                //break;
            }
            //Fill the model
            fillItemModel_availableNodes(availableNodeList[i],specParse->node );

        }
    }
    //parse all available components
    for (int i=0; i<availableCompList.count();i++){
        qDebug()<<availableCompList[i];
        if (QString(availableCompList[i]).endsWith(".launch")){

            //Fill the model
            fillItemModel_availableComponents(availableCompList[i]);

        }
    }
    ui->statusBar->showMessage(tr("Nodelist updated!"),5000);

}

void RxDev::on_pushButton_blankNodelet_clicked()
{
    NodeItem * newNode;
    newNode = new NodeItem("","nodelet",QStringList(""),QStringList(""),QStringList(""),QStringList(""));
    newNode->setType("nodelet");
    newNode->setArgs("load x on_y");
    newNode->setLocation(findSpace(QPoint(0,55)));
    newNode->setColor(Qt::yellow);
    newNode->setPos(findSpace(QPoint(0,55)));

    if (newNode->getNodeData()==true){
        scene->addItem(newNode);
    }
}

void RxDev::on_pushButton_managerNodelet_clicked()
{
    NodeItem * newNode;
    newNode = new NodeItem("","nodelet",QStringList(),QStringList(),QStringList(),QStringList());
    newNode->setType("nodelet");
    newNode->setArgs("manager");
    newNode->setLocation(findSpace(QPoint(0,55)));
    newNode->setColor(Qt::yellow);
    newNode->setPos(findSpace(QPoint(0,55)));

    if (newNode->getNodeData()==true){
        scene->addItem(newNode);
    }
}

void RxDev::on_pushButton_blankNode_clicked()
{
    NodeItem * newNode;
    newNode = new NodeItem("","",QStringList(),QStringList(),QStringList(),QStringList());
    newNode->setType("_blank_node");
    newNode->setLocation(findSpace(QPoint(0,55)));
newNode->setColor(Qt::yellow);
    newNode->setPos(findSpace(QPoint(0,55)));

    if (newNode->getNodeData()==true){
        scene->addItem(newNode);
    }


}


void RxDev::on_pushButton_group_clicked()
{
    GroupItem * newGroup;
    newGroup = new GroupItem;
    newGroup->setLocation(findSpace(QPoint(0,55)));
newGroup->setColor(Qt::yellow);
newGroup->setPos(findSpace(QPoint(0,55)));
    if (newGroup->getGroupData()==true){
        scene->addItem(newGroup);
    }

}

void RxDev::on_pushButton_env_clicked()
{
    EnvItem * newEnv;
    newEnv = new EnvItem;
    newEnv->setLocation(findSpace(QPoint(0,55)));
    newEnv->setPos(findSpace(QPoint(0,55)));
    newEnv->setColor(Qt::yellow);
    if (newEnv->getEnvData()==true){
        scene->addItem(newEnv);
    }

}

void RxDev::on_pushButton_arg_clicked()
{
    ArgItem * newArg;
    newArg = new ArgItem;
    newArg->setPos(findSpace(QPoint(0,55)));
    newArg->setLocation(findSpace(QPoint(0,55)));
    newArg->setColor(Qt::yellow);
    if (newArg->getArgData()==true){
        scene->addItem(newArg);
    }

}


void RxDev::on_pushButton_machine_clicked()
{
    MachineItem * newMachine;
    newMachine = new MachineItem;
    newMachine->setPos(findSpace(QPoint(0,55)));
    newMachine->setLocation(findSpace(QPoint(0,55)));
    newMachine->setColor(Qt::yellow);
    if (newMachine->getMachineData()==true){
        scene->addItem(newMachine);
    }

}


void RxDev::on_pushButton_Param_clicked()
{
    ParameterItem * newParam;
    newParam = new ParameterItem;
    newParam->setPos(findSpace(QPoint(0,0)));
    newParam->setLocation(findSpace(QPoint(0,55)));
    newParam->setColor(Qt::yellow);
    if (newParam->getParamData()==true){
        scene->addItem(newParam);
    }
}


void RxDev::on_pushButton_rosparam_clicked()
{
    RosparamItem * newParam;
    newParam = new RosparamItem;
    newParam->setPos(findSpace(QPoint(0,0)));
    newParam->setLocation(findSpace(QPoint(0,55)));
    newParam->setColor(Qt::yellow);
    if (newParam->getRosparamData()==true){
        scene->addItem(newParam);
    }
}


void RxDev::on_pushButton_remap_clicked()
{
    RemapItem* newRemap;
    newRemap = new RemapItem;
    newRemap->setPos(findSpace(QPoint(0,55)));
    newRemap->setLocation(findSpace(QPoint(0,55)));
    newRemap->setColor(Qt::yellow);
    if (newRemap->getRemapData()==true){
        scene->addItem(newRemap);
    }

}
void RxDev::on_pushButton_includeFile_clicked()
{
    IncludeFileItem * newFile;
    newFile = new IncludeFileItem;
    newFile->setPos(findSpace(QPoint(0,55)));
    connect(newFile,SIGNAL(expandItem(QString,GroupItem &)),this,SLOT(expandInclude(const QString &, GroupItem &)));
    newFile->setLocation(findSpace(QPoint(0,55)));
    newFile->setColor(Qt::yellow);
    if (newFile->getFileData()==true){
        scene->addItem(newFile);
    }

}

// way to simpel, should be improved to do a better distribution
QPointF RxDev::findSpace(QPointF currentPoint){
    //qDebug()<<"Auf "<<currentPoint<<" ist "<<scene->itemAt(currentPoint);
    if (scene->itemAt(currentPoint))
        return findSpace(QPointF(currentPoint.x()+1,currentPoint.y()));

    return currentPoint;
}

void RxDev::fillItemModel_availableComponents(const QString compFile)
{
    int begin = compFile.lastIndexOf("/")+1;
    int end = compFile.lastIndexOf(".launch");
    QString subString = compFile.mid(begin,end-begin);

    QStandardItem *group = new QStandardItem(QString("%1").arg(subString));
    QStandardItem *path = new QStandardItem(QString("%1").arg(compFile));
    QBrush b;
    b.setColor(Qt::darkBlue);
    group->setForeground(b);
    group->appendRow(path);

    // append group as new row to the model. model takes the ownership of the item
            model_availableComponents->appendRow(group);
}



void RxDev::showContextMenu_availableComponents(const QPoint&point){

    QMenu contextMenu_availableComponents;
    QModelIndex index(ui->listView_availableComponents->indexAt(point));
    //menuPoint_availableComponents = point;
    //const QModelIndex index = ui->listView_availableComponents->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    //qDebug()<<index;
    if(index.isValid()){
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();

    }
    contextMenu_availableComponents.addAction(tr("open Component File"),this, SLOT(openCompFile()));
    contextMenu_availableComponents.addSeparator();
    contextMenu_availableComponents.addAction(tr("add this Component"),this, SLOT(addCompFile()));
    //contextMenu_availableNodes.addAction(tr("expand all"),this, SLOT(expandAll()));
    //contextMenu_availableNodes.addAction(tr("collapse all"),this, SLOT(collapseAll()));



    contextMenu_availableComponents.exec(ui->listView_availableComponents->viewport()->mapToGlobal(point));
    }
}


void RxDev::showContextMenu_availableNodes(const QPoint&point){

    QMenu contextMenu_availableNodes;
    QModelIndex index(ui->treeView_availableNodes->indexAt(point));
    //menuPoint_availableNodes = point;
    //const QModelIndex index = ui->treeView_availableNodes->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    if(index.isValid()){

    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();

    }
    contextMenu_availableNodes.addAction(tr("view/edit Specfile"),this, SLOT(openSpecFile()));
    contextMenu_availableNodes.addAction(tr("expand"),this, SLOT(expandNode()));
    contextMenu_availableNodes.addAction(tr("collapse"),this, SLOT(collapseNode()));
    contextMenu_availableNodes.addSeparator();
    contextMenu_availableNodes.addAction(tr("expand all"),this, SLOT(expandAll()));
    contextMenu_availableNodes.addAction(tr("collapse all"),this, SLOT(collapseAll()));



    contextMenu_availableNodes.exec(ui->treeView_availableNodes->viewport()->mapToGlobal(point));
    }
}
void RxDev::expandNode(){
    const QModelIndex index = ui->treeView_availableNodes->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
    }
    ui->treeView_availableNodes->expand(seekRoot);
}
void RxDev::expandAll(){
    ui->treeView_availableNodes->expandAll();
}
void RxDev::collapseNode(){
    const QModelIndex index = ui->treeView_availableNodes->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
    }
    ui->treeView_availableNodes->collapse(seekRoot);
}
void RxDev::collapseAll(){
    ui->treeView_availableNodes->collapseAll();

}

void RxDev::openSpecFile(){
    const QModelIndex index = ui->treeView_availableNodes->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
    }
    QString filePath =  seekRoot.child(0,0).child(0,0).data(Qt::DisplayRole).toString();
    //qDebug()<<filePath;
    SpecFileParser *specParser = new SpecFileParser;
    specParser->nodeParser(filePath);

    SpecFileEdit specFile(&specParser->node);
    //qDebug()<<QString::fromStdString(specParser->node.type);
    specFile.setWindowTitle("Specfile: "+seekRoot.data(Qt::DisplayRole).toString());
    bool accept = specFile.exec();
    if ((accept)){
        QFile file;
        file.setFileName(filePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        if (file.isWritable()){
            QString tempContens = specParser->writeSpecFile(&specParser->node);
            QTextStream text(&file);
            text<<tempContens;
            //qDebug()<<"subs "<<specFile.getSubscriptions();
            file.close();
            QMessageBox::information( this, "File written!", "The file "+filePath+" was updated\n", QMessageBox::Ok, 0 );
            on_pushButton_refreshNodesOrComps_clicked();
        } else
            QMessageBox::critical(this, "File is write protected!", "The file "+filePath+" could not get updated\n", QMessageBox::Ok, 0 );

        //        setType(nodeEdit.getType());
        //        setName(nodeEdit.getName());
        //        setArgs(nodeEdit.getArgs());


    }
//    QDesktopServices::openUrl(QUrl::fromLocalFile( filePath));
}
void RxDev::openCompFile(){
    const QModelIndex index = ui->listView_availableComponents->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
    }
    QString filePath =  seekRoot.child(0,0).data(Qt::DisplayRole).toString();
  //  qDebug()<<filePath;
    QDesktopServices::openUrl(QUrl::fromLocalFile( filePath));
}
void RxDev::addCompFile(){
    const QModelIndex index = ui->listView_availableComponents->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
    }
    QString filePath =  seekRoot.child(0,0).data(Qt::DisplayRole).toString();
//    qDebug()<<filePath;
    IncludeFileItem * newFile;
    newFile = new IncludeFileItem;
    connect(newFile,SIGNAL(expandItem(QString,GroupItem &)),this,SLOT(expandInclude(const QString &, GroupItem &)));
    newFile->setPos(findSpace(QPoint(0,55)));
    newFile->setLocation(findSpace(QPoint(0,55)));
    newFile->setColor(Qt::yellow);
    newFile->setFile(filePath);
    //if (newFile->getFileData()==true){
        newFile->updateIncludeFileItem();
        scene->addItem(newFile);
    //}

}
template <typename InIt>
QStringList convertFromStdStringContainer(InIt begIt, InIt endIt){
   QStringList output;

   copy(begIt, endIt, output.begin(), output.end());
   return output;
}

void RxDev::selectionHandle_availableNodes(const QItemSelection &selected, const QItemSelection &deselected)
{
    const QModelIndex index = ui->treeView_availableNodes->selectionModel()->currentIndex();
    //find out the hierarchy level of the selected item
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();

    }
    SpecFileParser *specParser = new SpecFileParser;
    specParser->nodeParser(seekRoot.child(0,0).child(0,0).data(Qt::DisplayRole).toString());
    //  @todo Selected Node Datastructure
    gview->selectedNodeName = QString::fromStdString(specParser->node.type);
    gview->selectedNodePackage = QString::fromStdString(specParser->node.package);
    QStringList subs;
    for(std::list<Topic_Type>::iterator iter=specParser->node.subscriptions.begin();iter != specParser->node.subscriptions.end();iter++ )
    {
        subs.append(QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)));

    }
    gview->selectedNodeSubscriptions = subs;
    QStringList pubs;
    for(std::list<Topic_Type>::iterator iter=specParser->node.publications.begin();iter != specParser->node.publications.end();iter++ )
    {
        pubs.append(QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)));

    }
    gview->selectedNodePublications = pubs;
    QStringList servs;
    for(std::list<Topic_Type>::iterator iter=specParser->node.services.begin();iter != specParser->node.services.end();iter++ )
    {
        servs.append(QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)));
    }
    gview->selectedNodeServices = servs;
    QStringList params;
    for(std::list<Name_Type_Default>::iterator iter=specParser->node.parameters.begin();iter != specParser->node.parameters.end();iter++ )
    {
        params.append(QString("%1 %2 %3 %4").arg(QString::fromStdString(Name_Type_Default(*iter).paramName)).arg(QString::fromStdString(Name_Type_Default(*iter).paramType)).arg(QString::fromStdString(Name_Type_Default(*iter).paramDefault)).arg(QString::fromStdString(Name_Type_Default(*iter).paramRange)));

    }
    gview->selectedNodeParameters = params;
    //qDebug()<<QString("%1").arg(gview->selectedNodeName);
    //qDebug()<<QString("%1").arg(gview->selectedNodePackage);

    //if (selected == ))
    Q_UNUSED(deselected);

}

