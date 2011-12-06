#include "ui_machineEdit.h"
#include "machineEdit.h"
#include <QMessageBox>
#include <QDebug>

MachineEdit::MachineEdit(MachineItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::MachineEdit) {
    myItem=item;
    ui->setupUi(this);
    selectedEnv=-1;
    ui->toolButton_deleteEnv->setDefaultAction(ui->actionDelete_env);
    ui->toolButton_addEnv->setDefaultAction(ui->actionAdd_env);

    ui->lineEdit_name->setText(item->getName());
    ui->lineEdit_address->setText(item->getAddress());
    ui->lineEdit_ros_root->setText(item->getRos_root());
    ui->lineEdit_ros_package_path->setText(item->getRos_package_path());
    ui->lineEdit_default->setText(item->getDefault());
    ui->lineEdit_user->setText(item->getUser());
    ui->lineEdit_password->setText(item->getPassword());
    ui->lineEdit_timeout->setText(item->getTimeout());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    ui->lineEdit_name->setFocus();
    envModel = new QStandardItemModel(item->envItems.count(),4);
    envModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    envModel->setHeaderData(1,Qt::Horizontal,QObject::tr("value"));
    envModel->setHeaderData(2,Qt::Horizontal,QObject::tr("if"));
    envModel->setHeaderData(3,Qt::Horizontal,QObject::tr("unless"));
    fillEnvModel();
    ui->tableView_envItems->setModel(envModel);
    connect(ui->tableView_envItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_envItems(const QModelIndex &)));
    connect(ui->tableView_envItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_envItems(const QModelIndex &)));
}

void MachineEdit::selectionHandle_envItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedEnv=index.row();
    }
}
void MachineEdit::edit_envItems(const QModelIndex & index)
{
    EnvItem *env=myItem->envItems.at(index.row());
    env->getEnvData();
    fillEnvModel();
}

void MachineEdit::fillEnvModel(){
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

MachineEdit::~MachineEdit() {
    delete ui;
}

void MachineEdit::reject() {
    QDialog::reject();
}

void MachineEdit::accept() {

    if ( ui->lineEdit_name->text()=="" || ui->lineEdit_address->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No name or address specified</h2>"
                                                   "<p>Please insert the correct name and address!</p>"));
        ui->lineEdit_name->setFocus();

    } else{

        QDialog::accept();
    }

}

QString MachineEdit::getName(){
    return ui->lineEdit_name->text();
}
QString MachineEdit::getAddress(){
    return ui->lineEdit_address->text();

}

QString MachineEdit::getRos_root()
{
    return ui->lineEdit_ros_root->text();

}

QString MachineEdit::getRos_package_path()
{
    return ui->lineEdit_ros_package_path->text();

}

QString MachineEdit::getDefault()
{
    return ui->lineEdit_default->text();
}

QString MachineEdit::getUser()
{
    return ui->lineEdit_user->text();
}

QString MachineEdit::getPassword()
{
    return ui->lineEdit_password->text();

}

QString MachineEdit::getTimeout()
{
    return ui->lineEdit_timeout->text();
}

QString MachineEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString MachineEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}



void MachineEdit::on_actionDelete_env_triggered()
{
    myItem->removeEnvItem(myItem->envItems.at(selectedEnv));
    envModel->removeRow(selectedEnv);
    fillEnvModel();
}

void MachineEdit::on_actionAdd_env_triggered()
{
    EnvItem* newEnv;
      newEnv = new EnvItem;
      if (newEnv->getEnvData()==true){
          myItem->addEnvItem(newEnv);
      }
      fillEnvModel();
}
