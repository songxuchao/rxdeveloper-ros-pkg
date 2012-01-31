#include "ui_nodeParamEdit.h"
#include "nodeParamEdit.h"
#include <QMessageBox>
#include <QDebug>

NodeParamEdit::NodeParamEdit(QStringList list, QWidget *parent) : QDialog(parent), ui(new Ui::NodeParamEdit) {
    ui->setupUi(this);
    selectedParam=-1;
    paramModel = new QStandardItemModel(list.count(),4);
    paramModel->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    paramModel->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    paramModel->setHeaderData(2,Qt::Horizontal,QObject::tr("default"));
    paramModel->setHeaderData(3,Qt::Horizontal,QObject::tr("range"));

    for (int row = 0; row < list.count(); ++row) {
        QStringList tempList;
        QString tempString;
        tempList=list.at(row).split(QRegExp(" "));
        tempString=tempList.at(0);
//        qDebug()<<tempString;
        QStandardItem *item0 = new QStandardItem(tempString);
        paramModel->setItem(row,0,item0);
       tempString=tempList.at(1);
        item0 = new QStandardItem(QString(tempString));
        paramModel->setItem(row,1,item0);
        if (tempList.count()>2){
            tempString=tempList.at(2);
            item0 = new QStandardItem(QString(tempString));
        }else
            item0 = new QStandardItem(QString("-"));
        paramModel->setItem(row,2,item0);
        if (tempList.count()>3){
            tempString=tempList.at(3);
            item0 = new QStandardItem(QString(tempString));
        }else
            item0 = new QStandardItem(QString("-"));
        paramModel->setItem(row,3,item0);
        }

    ui->tableView_nodeParam->setModel(paramModel);
    connect(ui->tableView_nodeParam,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_paramItem(const QModelIndex &)));
    connect(ui->tableView_nodeParam,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectionHandle_paramItem_accepted(const QModelIndex &)));


}

NodeParamEdit::~NodeParamEdit() {
    delete ui;
}

void NodeParamEdit::selectionHandle_paramItem_accepted(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedParam=index.row();
        QDialog::accept();
    }
}
void NodeParamEdit::selectionHandle_paramItem(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedParam=index.row();
    }
}
void NodeParamEdit::reject() {

    QDialog::reject();
}

void NodeParamEdit::accept() {


    if ( selectedParam==-1){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Param missing</h2>"
                                                   "<p>Please choose a parameter!</p>"));

    } else{

        QDialog::accept();
    }

}

int NodeParamEdit::getParam(){

    return selectedParam;
}


