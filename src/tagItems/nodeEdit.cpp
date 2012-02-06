#include "ui_nodeEdit.h"
#include "nodeEdit.h"
#include <QMessageBox>
#include <QDebug>
#include "nodeParamEdit.h"


NodeEdit::NodeEdit(NodeItem *item, bool blank, QWidget *parent) : QDialog(parent), ui(new Ui::NodeEdit) {
    myItem=item;
    ui->setupUi(this);
    selectedEnv=-1;
    selectedRemap=-1;
    selectedParameter=-1;
    selectedRosparam=-1;
    if (blank){
        ui->lineEdit_pkg->setReadOnly(false);
        ui->lineEdit_type->setReadOnly(false);
    }
    ui->toolButton_deleteEnv->setDefaultAction(ui->actionDelete_env);
    ui->toolButton_addEnv->setDefaultAction(ui->actionAdd_env);
    ui->toolButton_deleteRemap->setDefaultAction(ui->actionDelete_remap);
    ui->toolButton_addRemap->setDefaultAction(ui->actionAdd_remap);
    ui->toolButton_deleteParam->setDefaultAction(ui->actionDelete_param);
    ui->toolButton_addParam->setDefaultAction(ui->actionAdd_param);
    ui->toolButton_deleteRosparam->setDefaultAction(ui->actionDelete_rosparam);
    ui->toolButton_addRosparam->setDefaultAction(ui->actionAdd_rosparam);

    ui->lineEdit_name->setText(item->getName());
    ui->lineEdit_pkg->setText(item->getPkg());
    ui->lineEdit_type->setText(item->getType());
    ui->lineEdit_args->setText(item->getArgs());
    ui->lineEdit_namespace->setText(item->getNamespace());
    ui->lineEdit_machine->setText(item->getMachine());
    ui->lineEdit_launch_prefix->setText(item->getLaunch_prefix());
    ui->comboBox_clear_params->setCurrentIndex(item->getClear_params());
    ui->comboBox_cwd->setCurrentIndex(item->getCwd());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    ui->comboBox_required->setCurrentIndex(item->getRequired());

    node_or_test = item->getNode_or_test();
    if (node_or_test==0){
        on_radioButton_standardNode_clicked();
        ui->comboBox_respawn->setCurrentIndex(item->getRespawn());
        ui->comboBox_output->setCurrentIndex(item->getOutput());
    }else{
        ui->radioButton_testNode->click();
        ui->lineEdit_time_limit->setText(item->getTime_limit());
        ui->lineEdit_retry->setText(item->getRetry());
    }
    ui->lineEdit_name->setFocus();

    remapModel = new QStandardItemModel(item->remapItems.count(),4);
    remapModel->setHeaderData(0,Qt::Horizontal,QObject::tr("from"));
    remapModel->setHeaderData(1,Qt::Horizontal,QObject::tr("to"));
    remapModel->setHeaderData(2,Qt::Horizontal,QObject::tr("if"));
    remapModel->setHeaderData(3,Qt::Horizontal,QObject::tr("unless"));
    fillRemapModel();
    ui->tableView_remapItems->setModel(remapModel);
    ui->tableView_remapItems->setColumnWidth(0,242);
    ui->tableView_remapItems->setColumnWidth(1,242);

    envModel = new QStandardItemModel(myItem->envItems.count(),4);
    envModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    envModel->setHeaderData(1,Qt::Horizontal,QObject::tr("value"));
    envModel->setHeaderData(2,Qt::Horizontal,QObject::tr("if"));
    envModel->setHeaderData(3,Qt::Horizontal,QObject::tr("unless"));
    fillEnvModel();
    ui->tableView_envItems->setModel(envModel);
    ui->tableView_envItems->setColumnWidth(0,242);
    ui->tableView_envItems->setColumnWidth(1,242);

    paramModel = new QStandardItemModel(myItem->paramItems.count(),5);
    paramModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    paramModel->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    paramModel->setHeaderData(2,Qt::Horizontal,QObject::tr("value"));
    paramModel->setHeaderData(3,Qt::Horizontal,QObject::tr("if"));
    paramModel->setHeaderData(4,Qt::Horizontal,QObject::tr("unless"));
    fillParameterModel();
    ui->tableView_paramItems->setModel(paramModel);
    ui->tableView_paramItems->setColumnWidth(0,200);
    ui->tableView_paramItems->setColumnWidth(1,200);
    ui->tableView_paramItems->setColumnWidth(2,170);
    ui->tableView_paramItems->setColumnWidth(3,54);
    ui->tableView_paramItems->setColumnWidth(4,60);

    rosparamModel = new QStandardItemModel(myItem->rosparamItems.count(),6);
    rosparamModel->setHeaderData(0,Qt::Horizontal,QObject::tr("command"));
    rosparamModel->setHeaderData(1,Qt::Horizontal,QObject::tr("file"));
    rosparamModel->setHeaderData(2,Qt::Horizontal,QObject::tr("param"));
    rosparamModel->setHeaderData(3,Qt::Horizontal,QObject::tr("namespace"));
    rosparamModel->setHeaderData(4,Qt::Horizontal,QObject::tr("if"));
    rosparamModel->setHeaderData(5,Qt::Horizontal,QObject::tr("unless"));
    fillRosparamModel();
    ui->tableView_rosparamItems->setModel(rosparamModel);
    ui->tableView_rosparamItems->setColumnWidth(0,200);
    ui->tableView_rosparamItems->setColumnWidth(1,200);
    ui->tableView_rosparamItems->setColumnWidth(2,170);
    ui->tableView_rosparamItems->setColumnWidth(3,54);
    ui->tableView_rosparamItems->setColumnWidth(4,60);


    connect(ui->tableView_paramItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_parameterItems(const QModelIndex &)));
    connect(ui->tableView_paramItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_parameterItems(const QModelIndex &)));
    connect(ui->tableView_rosparamItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_rosparamItems(const QModelIndex &)));
    connect(ui->tableView_rosparamItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_rosparamItems(const QModelIndex &)));
    connect(ui->tableView_envItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_envItems(const QModelIndex &)));
    connect(ui->tableView_envItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_envItems(const QModelIndex &)));
    connect(ui->tableView_remapItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_remapItems(const QModelIndex &)));
    connect(ui->tableView_remapItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_remapItems(const QModelIndex &)));

}
//Selection handler for the tableviews
void NodeEdit::edit_rosparamItems(const QModelIndex & index)
{
    RosparamItem *rosparam=myItem->rosparamItems.at(index.row());
    rosparam->getRosparamData();
    fillRosparamModel();
}
void NodeEdit::selectionHandle_rosparamItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedRosparam=index.row();
    }
}
void NodeEdit::edit_parameterItems(const QModelIndex & index)
{
    ParameterItem *param=myItem->paramItems.at(index.row());
    param->getParamData();
    fillParameterModel();
}
void NodeEdit::selectionHandle_parameterItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedParameter=index.row();
    }
}
void NodeEdit::edit_envItems(const QModelIndex & index)
{
    EnvItem *env=myItem->envItems.at(index.row());
    env->getEnvData();
    fillEnvModel();
}
void NodeEdit::selectionHandle_envItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedEnv=index.row();
    }
}
void NodeEdit::edit_remapItems(const QModelIndex & index)
{
    RemapItem *remap=myItem->remapItems.at(index.row());
    remap->getRemapData();
    fillRemapModel();
}
void NodeEdit::selectionHandle_remapItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedRemap=index.row();
    }
}
void NodeEdit::fillRemapModel(){
    for (int row = 0; row < myItem->remapItems.count(); ++row) {
        QStandardItem *item0 = new QStandardItem(QString(myItem->remapItems.at(row)->getFrom()));
        remapModel->setItem(row,0,item0);
        item0 = new QStandardItem(QString(myItem->remapItems.at(row)->getTo()));
        remapModel->setItem(row,1,item0);
        item0 = new QStandardItem(QString(myItem->remapItems.at(row)->getIf()));
        remapModel->setItem(row,2,item0);
        item0 = new QStandardItem(QString(myItem->remapItems.at(row)->getUnless()));
        remapModel->setItem(row,3,item0);
    }
}
void NodeEdit::fillParameterModel(){
    for (int row = 0; row < myItem->paramItems.count(); row++) {
        QStandardItem *item0 = new QStandardItem(QString(myItem->paramItems.at(row)->getName()));
        paramModel->setItem(row,0,item0);
        item0 = new QStandardItem(QString(myItem->paramItems.at(row)->getType()));
        paramModel->setItem(row,1,item0);
        item0 = new QStandardItem(QString(myItem->paramItems.at(row)->getValue()));
        paramModel->setItem(row,2,item0);
        item0 = new QStandardItem(QString(myItem->paramItems.at(row)->getIf()));
        paramModel->setItem(row,3,item0);
        item0 = new QStandardItem(QString(myItem->paramItems.at(row)->getUnless()));
        paramModel->setItem(row,4,item0);
    }
}
void NodeEdit::fillRosparamModel(){
    for (int row = 0; row < myItem->rosparamItems.count(); row++) {
        QStandardItem *item0 = new QStandardItem(QString(myItem->rosparamItems.at(row)->getName()));
        rosparamModel->setItem(row,2,item0);
        item0 = new QStandardItem(QString(myItem->rosparamItems.at(row)->getType()));
        rosparamModel->setItem(row,0,item0);
        item0 = new QStandardItem(QString(myItem->rosparamItems.at(row)->getValue()));
        rosparamModel->setItem(row,1,item0);
        item0 = new QStandardItem(QString(myItem->rosparamItems.at(row)->getNamespace()));
        rosparamModel->setItem(row,3,item0);
        item0 = new QStandardItem(QString(myItem->rosparamItems.at(row)->getIf()));
        rosparamModel->setItem(row,4,item0);
        item0 = new QStandardItem(QString(myItem->rosparamItems.at(row)->getUnless()));
        rosparamModel->setItem(row,5,item0);
    }
}

void NodeEdit::fillEnvModel(){
    for (int row = 0; row < myItem->envItems.count(); ++row) {
        QStandardItem *item0 = new QStandardItem(QString(myItem->envItems.at(row)->getName()));
        envModel->setItem(row,0,item0);
        item0 = new QStandardItem(QString(myItem->envItems.at(row)->getValue()));
        envModel->setItem(row,1,item0);
        item0 = new QStandardItem(QString(myItem->envItems.at(row)->getIf()));
        envModel->setItem(row,2,item0);
        item0 = new QStandardItem(QString(myItem->envItems.at(row)->getUnless()));
        envModel->setItem(row,3,item0);
    }
}

NodeEdit::~NodeEdit() {
    delete ui;
}

void NodeEdit::reject() {
    ui->lineEdit_name->setText("");
    QDialog::reject();
}

void NodeEdit::accept() {


    if ( ui->lineEdit_name->text()==""||ui->lineEdit_pkg->text()==""||ui->lineEdit_type->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Name, package or type missing</h2>"
                                                   "<p>Please insert the correct data!</p>"));
        ui->lineEdit_name->setFocus();

    } else{

        QDialog::accept();
    }

}

QString NodeEdit::getName(){
    return ui->lineEdit_name->text();
}

QString NodeEdit::getArgs(){
    return ui->lineEdit_args->text();
}
QString NodeEdit::getNamespace(){
    return ui->lineEdit_namespace->text();
}
QString NodeEdit::getMachine(){
    return ui->lineEdit_machine->text();
}
QString NodeEdit::getTime_limit(){
    return ui->lineEdit_time_limit->text();
}
QString NodeEdit::getRetry(){
    return ui->lineEdit_retry->text();
}
QString NodeEdit::getLaunch_prefix(){
    return ui->lineEdit_launch_prefix->text();
}
int NodeEdit::getRequired(){
    return ui->comboBox_required->currentIndex();
}

int NodeEdit::getRespawn(){
    return ui->comboBox_respawn->currentIndex();
}
int NodeEdit::getClear_params(){
    return ui->comboBox_clear_params->currentIndex();
}
int NodeEdit::getOutput(){
    return ui->comboBox_output->currentIndex();
}

int NodeEdit::getCwd(){
    return ui->comboBox_cwd->currentIndex();
}

void NodeEdit::on_radioButton_testNode_clicked()
{
    ui->radioButton_standardNode->setChecked(false);
    ui->label_name->setText("test-name:");
    ui->comboBox_respawn->setEnabled(false);
    ui->label_output->setEnabled(false);
    ui->label_machine->setEnabled(false);
    ui->comboBox_output->setEnabled(false);
    ui->lineEdit_machine->setEnabled(false);
    ui->label_time_limit->setEnabled(true);
    ui->lineEdit_time_limit->setEnabled(true);
    ui->label_retry->setEnabled(true);
    ui->lineEdit_retry->setEnabled(true);
    node_or_test = 1;

}

void NodeEdit::on_radioButton_standardNode_clicked()
{
    ui->radioButton_testNode->setChecked(false);
    ui->label_name->setText("name:");
    ui->comboBox_respawn->setEnabled(true);
    ui->label_output->setEnabled(true);
    ui->label_machine->setEnabled(true);
    ui->comboBox_output->setEnabled(true);
    ui->lineEdit_machine->setEnabled(true);
    ui->label_time_limit->setEnabled(false);
    ui->lineEdit_time_limit->setEnabled(false);
    ui->label_retry->setEnabled(false);
    ui->lineEdit_retry->setEnabled(false);
    node_or_test = 0;
}

QString NodeEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString NodeEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}

void NodeEdit::on_actionDelete_env_triggered()
{
    myItem->removeEnvItem(myItem->envItems.at(selectedEnv));
    envModel->removeRow(selectedEnv);
    fillEnvModel();
}

void NodeEdit::on_actionDelete_rosparam_triggered()
{
    myItem->removeRosparamItem(myItem->rosparamItems.at(selectedRosparam));
    rosparamModel->removeRow(selectedRosparam);
    fillRosparamModel();
}

void NodeEdit::on_actionDelete_param_triggered()
{
    myItem->removeParamItem(myItem->paramItems.at(selectedParameter));
    paramModel->removeRow(selectedParameter);
    fillParameterModel();
}

void NodeEdit::on_actionDelete_remap_triggered()
{
    myItem->removeRemapItem(myItem->remapItems.at(selectedRemap));
    remapModel->removeRow(selectedRemap);
    fillRemapModel();
}

void NodeEdit::on_actionAdd_param_triggered()
{
    QString topicString="",typeString="",defaultString="",rangeString="";
    if (!myItem->getParameters().isEmpty()){
//        qDebug()<<myItem->getParameters();
        NodeParamEdit nodeParam(myItem->getParameters());
        bool accept = nodeParam.exec();
        if (accept){
            QStringList tempList;
            tempList=myItem->getParameters().at(nodeParam.getParam()).split(QRegExp(" "));
            topicString=tempList.at(0);
            typeString=tempList.at(1);
            if (tempList.count()>2){
                defaultString=tempList.at(2);
            }
            if (tempList.count()>3){
                rangeString=tempList.at(3);
            }
        }
    }
    ParameterItem * newParam;
    newParam = new ParameterItem;
    newParam->setName(topicString);
    newParam->setType(typeString);
    newParam->setValue(defaultString);
    newParam->setRange(rangeString);
    if (newParam->getParamData()==true){
        myItem->addParamItem(newParam);
    }
    fillParameterModel();
}

void NodeEdit::on_actionAdd_env_triggered()
{
    EnvItem * newEnv;
    newEnv = new EnvItem;
    if (newEnv->getEnvData()==true){
        myItem->addEnvItem(newEnv);
    }
    fillEnvModel();
}

void NodeEdit::on_actionAdd_remap_triggered()
{
    RemapItem * newRemap;
    newRemap = new RemapItem;
    if (newRemap->getRemapData()==true){
        myItem->addRemapItem(newRemap);
    }
    fillRemapModel();
}

void NodeEdit::on_actionAdd_rosparam_triggered()
{
    RosparamItem * newRosparam;
    newRosparam = new RosparamItem;
    if (newRosparam->getRosparamData()==true){
        myItem->addRosparamItem(newRosparam);
    }
    fillRosparamModel();

}

QString NodeEdit::getType()
{
    return ui->lineEdit_type->text();
}

QString NodeEdit::getPackage()
{
    return ui->lineEdit_pkg->text();
}
