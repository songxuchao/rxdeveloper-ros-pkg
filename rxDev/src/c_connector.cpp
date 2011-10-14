#include "rxdev.h"
#include "ui_rxdev.h"
#include "c_connector.h"
#include "tagItems/parameterItem.h"
#include "tagItems/rosparamItem.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/groupItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/argItem.h"
#include "tagItems/envItem.h"
#include "tagItems/remapItem.h"
#include "std_msgs/String.h"
#include <yaml-cpp/yaml.h>
#include "launchFileView.h"
#include "launchFileScene.h"
#include <QUrl>


QPoint menuPoint_availableNodes;


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


void RxDev::availableNodes() {

    /* Create the data model */

    model_availableNodes  = new QStandardItemModel();

    ui->treeView_availableNodes->setModel(model_availableNodes);
    ui->treeView_availableNodes->setContextMenuPolicy(Qt::CustomContextMenu);

    on_pushButton_refreshNodes_clicked();

    connect(ui->treeView_availableNodes, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu_availableNodes(const QPoint&)));
    connect(ui->treeView_availableNodes->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this, SLOT(selectionHandle_availableNodes(const QItemSelection &,const QItemSelection &)));

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
    }
}


void RxDev::on_pushButton_refreshNodes_clicked()
{
    ui->statusBar->showMessage(tr("refreshing nodelist...please wait!"));
    packageList.clear();
    model_availableNodes->clear();
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
    }
    availableNodeList.removeDuplicates();
    //qDebug()<<availableNodeList;

    //parse all available specfiles
    for (int i=0; i<availableNodeList.count();i++){
        if (QString(availableNodeList[i]).endsWith(".node")){
            //qDebug()<<availableNodeList[i];

            //fetch nodespecs
            try{
            nodeParser(availableNodeList[i]);
            }catch(YAML::InvalidScalar &e){
            qDebug()<<"Invalid scalar in"<<QString(availableNodeList[i]);
            qDebug()<<e.what();
            return;
            }catch (YAML::ParserException&e){
             qDebug()<<"Parser exception in"<<QString(availableNodeList[i]);
             qDebug()<<e.what();
             return;
            }
            //Fill the model
            fillItemModel_availableNodes(availableNodeList[i]);

        }
    }

    ui->statusBar->showMessage(tr("Nodelist updated!"),5000);

}


void RxDev::on_pushButton_group_clicked()
{
    GroupItem * newGroup;
    newGroup = new GroupItem;
    newGroup->setLocation(findSpace(QPoint(0,55)));
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
    if (newRemap->getRemapData()==true){
        scene->addItem(newRemap);
    }

}
void RxDev::on_pushButton_includeFile_clicked()
{
    IncludeFileItem * newFile;
    newFile = new IncludeFileItem;
    newFile->setPos(findSpace(QPoint(0,55)));
    newFile->setLocation(findSpace(QPoint(0,55)));
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

void RxDev::fillItemModel_availableNodes(QString nodeFile){

    QStandardItem *group = new QStandardItem(QString("%1").arg(node.nodeType));

    QStandardItem *child;
    QStandardItem *item;

    child = new QStandardItem(QString("path"));
    item = new QStandardItem(QString("%1").arg(nodeFile));
    child->appendRow(item);
    // the appendRow function appends the child as new row

    group->appendRow(child);


    child = new QStandardItem(QString("type"));
    item = new QStandardItem(QString("%1").arg(node.nodeType));
    child->appendRow(item);
    child = new QStandardItem(QString("package"));
    item = new QStandardItem(QString("%1").arg(node.nodePackage));
    child->appendRow(item);

    group->appendRow(child);

    child = new QStandardItem(QString("subscriptions"));
    for (int j = 0; j<node.nodeInput.count(); j++)
    {
        item = new QStandardItem(QString("%1").arg(node.nodeInput[j]));
        child->appendRow(item);
    }

    group->appendRow(child);

    child = new QStandardItem(QString("publications"));

    QList<QStandardItem *> list;
    for (int j = 0; j<node.nodeOutput.count(); j++)
    {
        list.append(new QStandardItem(QString("%1").arg(node.nodeOutput[j])));

    }
    child->appendColumn(list);
    group->appendRow(child);

    child = new QStandardItem(QString("services"));
    for (int j = 0; j<node.nodeServices.count(); j++)
    {
        item = new QStandardItem(QString("%1").arg(node.nodeServices[j]));
        child->appendRow(item);
    }
    group->appendRow(child);

    child = new QStandardItem(QString("parameters"));
    for (int j = 0; j<node.nodeParameters.count(); j++)
    {
        item = new QStandardItem(QString("%1").arg(node.nodeParameters[j]));
        child->appendRow(item);
    }
    group->appendRow(child);

    // append group as new row to the model. model takes the ownership of the item
    if (!node.nodeType.isEmpty() && !node.nodePackage.isEmpty())
        model_availableNodes->appendRow(group);
}




/**
  * Nodespecfile-Parser
  **/
void RxDev::nodeParser(QString nodeFile){

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
    node.nodePackage.clear();
    node.nodeType.clear();
    node.nodeInput.clear();
    node.nodeOutput.clear();
    node.nodeServices.clear();
    node.nodeParameters.clear();

    //temp
    std::string nodePackage;
    std::string nodeType;
    std::string nodeInput;
    std::string nodeOutput;
    std::string nodeServices;
    std::string nodeParameters;
    //    if ((yamlNode = doc.FindValue("package"))) {
    //        *yamlNode >> nodePackage;
    //        if (nodePackage!="~"){

    //            node.nodeInput = QString::fromStdString(nodeInput).split(" ");
    //        }
    //    }


    try {
        doc["type"] >> nodeType;
        node.nodeType = QString::fromStdString(nodeType);

    } catch (YAML::InvalidScalar) {
        qDebug()<<"No valid type found";
        exit(-1);

    }
    if ((yamlNode = doc.FindValue("package"))) {
        *yamlNode >> nodePackage;
        node.nodePackage = QString::fromStdString(nodePackage);
    } else{
        qDebug()<<"No valid package found in "+nodeFile;

    }
    if ((yamlNode = doc.FindValue("subscriptions"))) {
        *yamlNode >> nodeInput;
        if (nodeInput!="~"){

            node.nodeInput = QString::fromStdString(nodeInput).split(" ");
        }
    }
    if ((yamlNode = doc.FindValue("publications"))) {
        *yamlNode >> nodeOutput;
        if (nodeOutput!="~"){
            node.nodeOutput = QString::fromStdString(nodeOutput).split(" ");
        }

    }
    if ((yamlNode = doc.FindValue("services"))) {
        *yamlNode >> nodeServices;
        if (nodeServices!="~"){
            node.nodeServices = QString::fromStdString(nodeServices).split(" ");
        }

    }
    if ((yamlNode = doc.FindValue("parameters"))) {
        *yamlNode >> nodeParameters;
        if (nodeParameters!="~"){
            node.nodeParameters = QString::fromStdString(nodeParameters).split(" ");
        }
    }

}



void RxDev::showContextMenu_availableNodes(const QPoint&point){

    QMenu contextMenu_availableNodes;
    //QModelIndex index(ui->treeView_availableNodes->indexAt(point));
    menuPoint_availableNodes = point;
    const QModelIndex index = ui->treeView_availableNodes->selectionModel()->currentIndex();
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();

    }
    contextMenu_availableNodes.addAction(tr("open Specfile"),this, SLOT(openSpecFile()));
    contextMenu_availableNodes.addAction(tr("expand"),this, SLOT(expandNode()));
    contextMenu_availableNodes.addAction(tr("collapse"),this, SLOT(collapseNode()));
    contextMenu_availableNodes.addSeparator();
    contextMenu_availableNodes.addAction(tr("expand all"),this, SLOT(expandAll()));
    contextMenu_availableNodes.addAction(tr("collapse all"),this, SLOT(collapseAll()));



    contextMenu_availableNodes.exec(ui->treeView_availableNodes->viewport()->mapToGlobal(point));
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
    QDesktopServices::openUrl(QUrl::fromLocalFile( filePath));
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
    nodeParser(seekRoot.child(0,0).child(0,0).data(Qt::DisplayRole).toString());
    gview->selectedNodeName = node.nodeType;
    gview->selectedNodePackage = node.nodePackage;
    gview->selectedNodeSubscriptions = node.nodeInput;
    gview->selectedNodePublications = node.nodeOutput;
    gview->selectedNodeServices = node.nodeServices;
    gview->selectedNodeParameters = node.nodeParameters;
    qDebug()<<QString("%1").arg(gview->selectedNodeName);
    qDebug()<<QString("%1").arg(gview->selectedNodePackage);

    //if (selected == ))
    Q_UNUSED(deselected);

}

