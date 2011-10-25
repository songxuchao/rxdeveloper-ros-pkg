#include "ui_remapArrowEdit.h"
#include "remapArrowEdit.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDebug>
#include "groupItem.h"

RemapArrowEdit::RemapArrowEdit(RemapArrow *item, QWidget *parent) : QDialog(parent), ui(new Ui::RemapArrowEdit) {
    ui->setupUi(this);
    ui->groupBox->setTitle("From node: "+item->startItem()->getType());
    ui->groupBox_2->setTitle("To node: "+item->endItem()->getType());
    myItem=item;
    fromNode="";
    fromNodeNamespace="";
    fromGroupNamespace="";
    toNode="";
    toNodeNamespace="";
    toGroupNamespace="";
    fromString=item->getFrom();
    toString=item->getTo();

    if (toString==""){
      on_checkBox_targetName_toggled(true);
        ui->checkBox_targetName->setChecked(true);
        if (myItem->endItem()->parentItem()){
            ui->checkBox_targetGroupNs->setChecked(true);
            on_checkBox_targetNamespace_toggled(true);
        }
    }
    if (item->getFrom().contains(QString(myItem->startItem()->getName()).append("/"))){
        ui->checkBox_startName->setChecked(true);
        fromString.remove(myItem->startItem()->getName().append("/"));
    }
    if (myItem->startItem()->getNamespace()!=""&&item->getFrom().contains(QString(myItem->startItem()->getNamespace()).append("/"))){
            ui->checkBox_startNamespace->setChecked(true);
            fromString.remove(myItem->startItem()->getNamespace().append("/"));
    }
    if (myItem->startItem()->parentItem()){
        GroupItem *group = qgraphicsitem_cast<GroupItem *>(myItem->startItem()->parentItem());
        if (group->getNamespace()!=""&&item->getFrom().contains(QString(group->getNamespace()).append("/"))){
            ui->checkBox_startGroupNs->setChecked(true);
            fromString.remove(group->getNamespace().append("/"));
        }
    }
    if (item->getTo().contains(QString(myItem->endItem()->getName()).append("/"))){
        ui->checkBox_targetName->setChecked(true);
        toString.remove(myItem->endItem()->getName().append("/"));
    }
    if (myItem->endItem()->getNamespace()!=""&&item->getTo().contains(QString(myItem->endItem()->getNamespace()).append("/"))){
            ui->checkBox_targetNamespace->setChecked(true);
            toString.remove(myItem->endItem()->getNamespace().append("/"));
    }
    if (myItem->endItem()->parentItem()){
        GroupItem *group = qgraphicsitem_cast<GroupItem *>(myItem->endItem()->parentItem());
        if (group->getNamespace()!=""&&item->getTo().contains(QString(group->getNamespace()).append("/"))){
            ui->checkBox_targetGroupNs->setChecked(true);
            toString.remove(group->getNamespace().append("/"));

        }

    }

    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    model_start_subscribe = new QStringListModel;
    model_start_subscribe->setStringList(item->startItem()->getSubscriptions());
    ui->listView_startNode_subscribe->setModel(model_start_subscribe);
    model_end_publish = new QStringListModel;
    model_end_publish->setStringList(item->endItem()->getPublications());
    ui->listView_endNode_publish->setModel(model_end_publish);
    model_start_publish = new QStringListModel;
    model_start_publish->setStringList(item->startItem()->getPublications());
    ui->listView_startNode_publish->setModel(model_start_publish);
    model_end_subscribe = new QStringListModel;
    model_end_subscribe->setStringList(item->endItem()->getSubscriptions());
    ui->listView_endNode_subscribe->setModel(model_end_subscribe);
    ui->lineEdit_from->setText(item->getFrom());
    ui->lineEdit_to->setText(item->getTo());
    connect(ui->listView_startNode_subscribe->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_start_subscribe(const QItemSelection &,const QItemSelection &)));
    connect(ui->listView_endNode_publish->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_end_publish(const QItemSelection &,const QItemSelection &)));
    connect(ui->listView_startNode_publish->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_start_publish(const QItemSelection &,const QItemSelection &)));
    connect(ui->listView_endNode_subscribe->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_end_subscribe(const QItemSelection &,const QItemSelection &)));
}

RemapArrowEdit::~RemapArrowEdit() {
    delete ui;
}
void RemapArrowEdit::selectionHandle_start_subscribe(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->listView_startNode_publish->selectionModel()->clearSelection();
    ui->listView_endNode_subscribe->selectionModel()->clearSelection();
    QModelIndex index;
    index =  ui->listView_startNode_subscribe->selectionModel()->currentIndex();
    fromString=(index.data(Qt::DisplayRole).toString());
    int chopIndex = fromString.indexOf("{");
    fromString.chop(fromString.count()-chopIndex);
    updateFrom();
}


void RemapArrowEdit::selectionHandle_end_publish(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->listView_startNode_publish->selectionModel()->clearSelection();
    ui->listView_endNode_subscribe->selectionModel()->clearSelection();
    QModelIndex index;
    index =  ui->listView_endNode_publish->selectionModel()->currentIndex();
    toString =( index.data(Qt::DisplayRole).toString());
    int chopIndex = toString.indexOf("{");
    toString.chop(toString.count()-chopIndex);

    updateTo();
}
void RemapArrowEdit::selectionHandle_start_publish(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->listView_startNode_subscribe->selectionModel()->clearSelection();
    ui->listView_endNode_publish->selectionModel()->clearSelection();
    QModelIndex index;
    index =  ui->listView_startNode_publish->selectionModel()->currentIndex();
    fromString=(index.data(Qt::DisplayRole).toString());
    int chopIndex = fromString.indexOf("{");
    fromString.chop(fromString.count()-chopIndex);

    updateFrom();
}


void RemapArrowEdit::selectionHandle_end_subscribe(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->listView_startNode_subscribe->selectionModel()->clearSelection();
    ui->listView_endNode_publish->selectionModel()->clearSelection();
    QModelIndex index;
    index =  ui->listView_endNode_subscribe->selectionModel()->currentIndex();
    toString =( index.data(Qt::DisplayRole).toString());
    int chopIndex = toString.indexOf("{");
    toString.chop(toString.count()-chopIndex);
    updateTo();
}


void RemapArrowEdit::reject() {
    QDialog::reject();
}

void RemapArrowEdit::accept() {
    //if no row selected reject
    if (ui->lineEdit_from->text()==""||ui->lineEdit_to->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Missing remap information</h2>"
                                                   "<p>Please select a row on both sides or fill in the right information.</p>"));
    } else{
        if (ui->listView_startNode_subscribe->selectionModel()->currentIndex().isValid())
            subToPub=false;
        if (ui->listView_startNode_publish->selectionModel()->currentIndex().isValid())
            subToPub=true;

        QDialog::accept();
    }

}

QString RemapArrowEdit::getFrom(){
    return ui->lineEdit_from->text();
}

QString RemapArrowEdit::getTo(){

    return ui->lineEdit_to->text();
}

bool RemapArrowEdit::getSubToPub()
{
    return subToPub;
}

QString RemapArrowEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString RemapArrowEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}

void RemapArrowEdit::updateFrom()
{

    ui->lineEdit_from->setText(fromGroupNamespace+fromNodeNamespace+fromNode+fromString);
    while(ui->lineEdit_from->text().contains("//")){
        QString temp =ui->lineEdit_from->text();
        int index = temp.indexOf("//");

        temp.remove(index,1);
        ui->lineEdit_from->setText(temp);
    }
}

void RemapArrowEdit::updateTo()
{
    ui->lineEdit_to->setText(toGroupNamespace+toNodeNamespace+toNode+toString);
    if (ui->lineEdit_to->text().contains("//")){
        QString temp =ui->lineEdit_to->text();
        int index = temp.indexOf("//");
        temp.remove(index,1);
        ui->lineEdit_to->setText(temp);
    }

}

void RemapArrowEdit::on_checkBox_startName_toggled(bool checked)
{
    if (checked)
        fromNode= QString("/").append(myItem->startItem()->getName()).append("/");
    else
        fromNode="";
    updateFrom();
}

void RemapArrowEdit::on_checkBox_targetName_toggled(bool checked)
{
    if (checked)
        toNode= QString("/").append(myItem->endItem()->getName()).append("/");
    else
        toNode="";
    updateTo();
}

void RemapArrowEdit::on_checkBox_startNamespace_toggled(bool checked)
{
    if (checked&&myItem->startItem()->getNamespace()!="")
            fromNodeNamespace= QString("/").append(myItem->startItem()->getNamespace()).append("/");
    else
        fromNodeNamespace="";
    updateFrom();
}

void RemapArrowEdit::on_checkBox_targetNamespace_toggled(bool checked)
{
    if (checked&&myItem->endItem()->getNamespace()!="")
            toNodeNamespace= QString("/").append(myItem->endItem()->getNamespace()).append("/");
    else
        toNodeNamespace="";
    updateTo();
}

void RemapArrowEdit::on_checkBox_startGroupNs_toggled(bool checked)
{
    if (checked&&myItem->startItem()->parentItem()){
            GroupItem *group = qgraphicsitem_cast<GroupItem *>(myItem->startItem()->parentItem());
            if (group->getNamespace()!="")
                fromGroupNamespace.prepend(QString("/").append(group->getNamespace()).append("/"));
        }
    else
        fromGroupNamespace="";
    updateFrom();
}

void RemapArrowEdit::on_checkBox_targetGroupNs_toggled(bool checked)
{
    if (checked&&myItem->endItem()->parentItem()){
            GroupItem *group = qgraphicsitem_cast<GroupItem *>(myItem->endItem()->parentItem());
            if (group->getNamespace()!="")
                toGroupNamespace.prepend(QString("/").append(group->getNamespace()).append("/"));
        }
    else
        toGroupNamespace="";
    updateTo();
}
