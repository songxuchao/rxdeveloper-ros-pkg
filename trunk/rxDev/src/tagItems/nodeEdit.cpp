#include "ui_nodeEdit.h"
#include "nodeEdit.h"
#include <QMessageBox>
#include <QDebug>

NodeEdit::NodeEdit(NodeItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::NodeEdit) {
    ui->setupUi(this);
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

    node_or_test = item->getNode_or_test();
    if (node_or_test==0){
        on_radioButton_standardNode_clicked();
        ui->comboBox_required->setCurrentIndex(item->getRequired());
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
    for (int row = 0; row < item->remapItems.count(); ++row) {
        QStandardItem *item0 = new QStandardItem(QString(item->remapItems.at(row)->getFrom()));
        remapModel->setItem(row,0,item0);
        item0 = new QStandardItem(QString(item->remapItems.at(row)->getTo()));
        remapModel->setItem(row,1,item0);
        item0 = new QStandardItem(QString(item->remapItems.at(row)->getIf()));
        remapModel->setItem(row,2,item0);
        item0 = new QStandardItem(QString(item->remapItems.at(row)->getUnless()));
        remapModel->setItem(row,3,item0);

    }
    ui->tableView_remapItems->setModel(remapModel);


    envModel = new QStandardItemModel(item->envItems.count(),4);
    envModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    envModel->setHeaderData(1,Qt::Horizontal,QObject::tr("value"));
    envModel->setHeaderData(2,Qt::Horizontal,QObject::tr("if"));
    envModel->setHeaderData(3,Qt::Horizontal,QObject::tr("unless"));
    for (int row = 0; row < item->envItems.count(); ++row) {
        QStandardItem *item0 = new QStandardItem(QString(item->envItems.at(row)->getName()));
        envModel->setItem(row,0,item0);
        item0 = new QStandardItem(QString(item->envItems.at(row)->getValue()));
        envModel->setItem(row,1,item0);
        item0 = new QStandardItem(QString(item->envItems.at(row)->getIf()));
        envModel->setItem(row,2,item0);
        item0 = new QStandardItem(QString(item->envItems.at(row)->getUnless()));
        envModel->setItem(row,3,item0);

    }
    ui->tableView_envItems->setModel(envModel);


    int paramCount=0;
    int rosparamCount=0;

    for (int row = 0; row < item->paramItems.count(); row++) {
        if (item->paramItems.at(row)->getStandardParameter()<3)
            paramCount++;
        else
            rosparamCount++;
    }
    paramModel = new QStandardItemModel(paramCount,5);
    paramModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    paramModel->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    paramModel->setHeaderData(2,Qt::Horizontal,QObject::tr("value"));
    paramModel->setHeaderData(3,Qt::Horizontal,QObject::tr("if"));
    paramModel->setHeaderData(4,Qt::Horizontal,QObject::tr("unless"));

    rosparamModel = new QStandardItemModel(rosparamCount,6);
    rosparamModel->setHeaderData(0,Qt::Horizontal,QObject::tr("command"));
    rosparamModel->setHeaderData(1,Qt::Horizontal,QObject::tr("file"));
    rosparamModel->setHeaderData(2,Qt::Horizontal,QObject::tr("param"));
    rosparamModel->setHeaderData(3,Qt::Horizontal,QObject::tr("namespace"));
    rosparamModel->setHeaderData(4,Qt::Horizontal,QObject::tr("if"));
    rosparamModel->setHeaderData(5,Qt::Horizontal,QObject::tr("unless"));

    for (int row = 0; row < item->paramItems.count(); row++) {
        if (item->paramItems.at(row)->getStandardParameter()<3){

            QStandardItem *item0 = new QStandardItem(QString(item->paramItems.at(row)->getName()));
            paramModel->setItem(row,0,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getType()));
            paramModel->setItem(row,1,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getValue()));
            paramModel->setItem(row,2,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getIf()));
            paramModel->setItem(row,3,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getUnless()));
            paramModel->setItem(row,4,item0);
        }
    }
    for (int row = 0; row < item->paramItems.count(); row++) {
        if (item->paramItems.at(row)->getStandardParameter()==3){
            QStandardItem *item0 = new QStandardItem(QString(item->paramItems.at(row)->getName()));
            rosparamModel->setItem(row,2,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getType()));
            rosparamModel->setItem(row,0,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getValue()));
            rosparamModel->setItem(row,1,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getNamespace()));
            rosparamModel->setItem(row,3,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getIf()));
            rosparamModel->setItem(row,4,item0);
            item0 = new QStandardItem(QString(item->paramItems.at(row)->getUnless()));
            rosparamModel->setItem(row,5,item0);

        }
    }
    ui->tableView_paramItems->setModel(paramModel);

    ui->tableView_rosparamItems->setModel(rosparamModel);

}

NodeEdit::~NodeEdit() {
    delete ui;
}

void NodeEdit::reject() {
    ui->lineEdit_name->setText("");
    QDialog::reject();
}

void NodeEdit::accept() {


    if ( ui->lineEdit_name->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Name missing</h2>"
                                                   "<p>Please insert the correct name!</p>"));
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
    ui->comboBox_required->setEnabled(false);
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
    ui->comboBox_required->setEnabled(true);
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
