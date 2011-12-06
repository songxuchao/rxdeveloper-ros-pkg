#include "ui_includeFileEdit.h"
#include "includeFileEdit.h"
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>

IncludeFileEdit::IncludeFileEdit(IncludeFileItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::IncludeFileEdit) {
    myItem=item;
    ui->setupUi(this);
    expandFile="";
    selectedEnv=-1;
    selectedArg=-1;
    ui->toolButton_deleteEnv->setDefaultAction(ui->actionDelete_env);
    ui->toolButton_addEnv->setDefaultAction(ui->actionAdd_env);
    ui->toolButton_deleteArg->setDefaultAction(ui->actionDelete_arg);
    ui->toolButton_addArg->setDefaultAction(ui->actionAdd_arg);

    ui->lineEdit_file->setText(item->getFile());
    ui->lineEdit_namespace->setText(item->getNamespace());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    ui->comboBox_clearParams->setCurrentIndex(item->getClearParams());
    envModel = new QStandardItemModel(item->envItems.count(),4);
    envModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    envModel->setHeaderData(1,Qt::Horizontal,QObject::tr("value"));
    envModel->setHeaderData(2,Qt::Horizontal,QObject::tr("if"));
    envModel->setHeaderData(3,Qt::Horizontal,QObject::tr("unless"));
    fillEnvModel();
    ui->tableView_envItems->setModel(envModel);

    argModel = new QStandardItemModel(item->argItems.count(),5);
    argModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    argModel->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    argModel->setHeaderData(2,Qt::Horizontal,QObject::tr("value"));
    argModel->setHeaderData(3,Qt::Horizontal,QObject::tr("if"));
    argModel->setHeaderData(4,Qt::Horizontal,QObject::tr("unless"));
    fillArgModel();
    ui->tableView_argItems->setModel(argModel);
    connect(ui->tableView_envItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_envItems(const QModelIndex &)));
    connect(ui->tableView_envItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_envItems(const QModelIndex &)));
    connect(ui->tableView_argItems,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edit_argItems(const QModelIndex &)));
    connect(ui->tableView_argItems,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_argItems(const QModelIndex &)));
}

void IncludeFileEdit::selectionHandle_envItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedEnv=index.row();
    }
}

void IncludeFileEdit::edit_envItems(const QModelIndex & index)
{
    EnvItem *env=myItem->envItems.at(index.row());
    env->getEnvData();
    fillEnvModel();
}

void IncludeFileEdit::selectionHandle_argItems(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedArg=index.row();
    }
}
void IncludeFileEdit::edit_argItems(const QModelIndex & index)
{
    ArgItem *arg=myItem->argItems.at(index.row());
    arg->getArgData();
    fillArgModel();
}

void IncludeFileEdit::fillEnvModel(){
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
void IncludeFileEdit::fillArgModel(){

    for (int row = 0; row < myItem->argItems.count(); ++row) {
        QStandardItem *item0 = new QStandardItem(QString(myItem->argItems.at(row)->getName()));
        argModel->setItem(row,0,item0);
        switch (myItem->argItems.at(row)->getValue_or_default()){
        case 0:
            item0 = new QStandardItem(QString("-"));
            argModel->setItem(row,1,item0);
            item0 = new QStandardItem(QString("-"));
            argModel->setItem(row,2,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getIf()));
            argModel->setItem(row,3,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getUnless()));
            argModel->setItem(row,4,item0);
            break;
        case 1:
            item0 = new QStandardItem(QString("value"));
            argModel->setItem(row,1,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getValue()));
            argModel->setItem(row,2,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getIf()));
            argModel->setItem(row,3,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getUnless()));
            argModel->setItem(row,4,item0);
            break;
        case 2:
            item0 = new QStandardItem(QString("default"));
            argModel->setItem(row,1,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getValue()));
            argModel->setItem(row,2,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getIf()));
            argModel->setItem(row,3,item0);
            item0 = new QStandardItem(QString(myItem->argItems.at(row)->getUnless()));
            argModel->setItem(row,4,item0);
            break;
        }
    }
}

IncludeFileEdit::~IncludeFileEdit() {
    delete ui;
}

void IncludeFileEdit::reject() {
    ui->lineEdit_file->setText("");
    QDialog::reject();
}

void IncludeFileEdit::accept() {

    if ( ui->lineEdit_file->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No file specified</h2>"
                                                   "<p>Please insert the correct file path!</p>"));
        ui->lineEdit_file->setFocus();

    } else{

        QDialog::accept();
    }

}

QString IncludeFileEdit::getFile(){
    return ui->lineEdit_file->text();
}
QString IncludeFileEdit::getNamespace(){
    return ui->lineEdit_namespace->text();

}
int IncludeFileEdit::getClearParam()
{
    return ui->comboBox_clearParams->currentIndex();
}
QString IncludeFileEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString IncludeFileEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}


void IncludeFileEdit::on_actionDelete_env_triggered()
{
    myItem->removeEnvItem(myItem->envItems.at(selectedEnv));
    envModel->removeRow(selectedEnv);
    fillEnvModel();
}

void IncludeFileEdit::on_actionDelete_arg_triggered()
{
    myItem->removeArgItem(myItem->argItems.at(selectedArg));
    argModel->removeRow(selectedArg);
    fillEnvModel();
}

void IncludeFileEdit::on_actionAdd_arg_triggered()
{
    ArgItem* newArg;
      newArg = new ArgItem;
      if (newArg->getArgData()==true){
          myItem->addArgItem(newArg);
      }
      fillArgModel();
}

void IncludeFileEdit::on_actionAdd_env_triggered()
{
    EnvItem* newEnv;
      newEnv = new EnvItem;
      if (newEnv->getEnvData()==true){
          myItem->addEnvItem(newEnv);
      }
      fillEnvModel();
}
void IncludeFileEdit::on_pushButton_tryOpen_clicked()
{
    QString file = ui->lineEdit_file->text();
    if (file.startsWith("$(find ")){
        file.remove("$(find ");
        QStringList tempList;
        tempList<<file.split(")");
        QProcess findPath;
        findPath.start(QString("rospack find "+tempList.at(0)));
        findPath.waitForFinished(-1);
        QByteArray output = findPath.readAllStandardOutput();
        file = output.trimmed();
        file.append(tempList.at(1));
    }else{
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(file));

}

QString IncludeFileEdit::getExpandFile()
{
    return expandFile;
}

void IncludeFileEdit::on_checkBox_expand_toggled(bool checked)
{
    if (checked){
        expandFile = ui->lineEdit_file->text();
        expandFile.trimmed();
        if (expandFile.startsWith("$(find ")){
            expandFile.remove("$(find ");
            QStringList tempList;
            tempList<<expandFile.split(")");
            QProcess findPath;
            findPath.start(QString("rospack find "+tempList.at(0)));
            findPath.waitForFinished(-1);
            QByteArray output = findPath.readAllStandardOutput();
            expandFile = output.trimmed();
            expandFile.append(tempList.at(1));
        }
    }else
        expandFile = "";
}
