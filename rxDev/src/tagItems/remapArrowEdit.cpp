#include "ui_remapArrowEdit.h"
#include "remapArrowEdit.h"
#include <QMessageBox>
#include <QStandardItemModel>

RemapArrowEdit::RemapArrowEdit(RemapArrow *item, QWidget *parent) : QDialog(parent), ui(new Ui::RemapArrowEdit) {
    ui->setupUi(this);
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
    connect(ui->listView_startNode_subscribe->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_start_subscribe(const QItemSelection &,const QItemSelection &)));
    connect(ui->listView_endNode_publish->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_end_publish(const QItemSelection &,const QItemSelection &)));
    connect(ui->listView_startNode_publish->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_start_publish(const QItemSelection &,const QItemSelection &)));
    connect(ui->listView_endNode_subscribe->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)), this, SLOT(selectionHandle_end_subscribe(const QItemSelection &,const QItemSelection &)));


    //fill both lists
   //  ui->lineEdit_file->setText(item->getFile());
  //   ui->lineEdit_namespace->setText(item->getNamespace());

}

RemapArrowEdit::~RemapArrowEdit() {
    delete ui;
}
void RemapArrowEdit::selectionHandle_start_subscribe(const QItemSelection &selected, const QItemSelection &deselected)
{
      ui->listView_startNode_publish->selectionModel()->clearSelection();
      ui->listView_endNode_subscribe->selectionModel()->clearSelection();
}


void RemapArrowEdit::selectionHandle_end_publish(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->listView_startNode_publish->selectionModel()->clearSelection();
    ui->listView_endNode_subscribe->selectionModel()->clearSelection();
}
void RemapArrowEdit::selectionHandle_start_publish(const QItemSelection &selected, const QItemSelection &deselected)
{
      ui->listView_startNode_subscribe->selectionModel()->clearSelection();
      ui->listView_endNode_publish->selectionModel()->clearSelection();
}


void RemapArrowEdit::selectionHandle_end_subscribe(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->listView_startNode_subscribe->selectionModel()->clearSelection();
    ui->listView_endNode_publish->selectionModel()->clearSelection();
}


void RemapArrowEdit::reject() {
    QDialog::reject();
}

void RemapArrowEdit::accept() {
    //if no row selected reject
    if ((!ui->listView_startNode_subscribe->selectionModel()->currentIndex().isValid()
         || !ui->listView_endNode_publish->selectionModel()->currentIndex().isValid())
            && (!ui->listView_startNode_publish->selectionModel()->currentIndex().isValid()
                || !ui->listView_endNode_subscribe->selectionModel()->currentIndex().isValid() )){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Missing remap information</h2>"
                                                   "<p>Please select a row on both sides.</p>"));
    } else{
    if (ui->listView_startNode_subscribe->selectionModel()->currentIndex().isValid())
        subToPub=false;
    if (ui->listView_startNode_publish->selectionModel()->currentIndex().isValid())
        subToPub=true;

        QDialog::accept();
    }

}

QString RemapArrowEdit::getFrom(){
    QModelIndex index;
    if (ui->listView_startNode_subscribe->selectionModel()->currentIndex().isValid())
         index =  ui->listView_startNode_subscribe->selectionModel()->currentIndex();

    if (ui->listView_startNode_publish->selectionModel()->currentIndex().isValid())
         index =  ui->listView_startNode_publish->selectionModel()->currentIndex();

    return index.data(Qt::DisplayRole).toString();
}

QString RemapArrowEdit::getTo(){
    QModelIndex index;
    if (ui->listView_endNode_publish->selectionModel()->currentIndex().isValid())
        index =  ui->listView_endNode_publish->selectionModel()->currentIndex();
    if (ui->listView_endNode_subscribe->selectionModel()->currentIndex().isValid())
        index =  ui->listView_endNode_subscribe->selectionModel()->currentIndex();
    return index.data(Qt::DisplayRole).toString();
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
