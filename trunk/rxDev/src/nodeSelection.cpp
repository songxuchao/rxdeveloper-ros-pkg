#include "ui_nodeSelection.h"
#include "nodeSelection.h"
#include <QMessageBox>
#include <QDebug>

NodeSelection::NodeSelection(QStringList list, QWidget *parent) : QDialog(parent), ui(new Ui::NodeSelection) {
    ui->setupUi(this);
    selectedNode=-1;
    nodeModel = new QStringListModel();
    nodeModel->setStringList(list);
    ui->listView->setModel(nodeModel);
    connect(ui->listView,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_nodeItem(const QModelIndex &)));
    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectionHandle_nodeItem_accepted(const QModelIndex &)));


}

NodeSelection::~NodeSelection() {
    delete ui;
}

void NodeSelection::selectionHandle_nodeItem_accepted(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedNode=index.row();
        QDialog::accept();
    }
}
void NodeSelection::selectionHandle_nodeItem(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedNode=index.row();
    }
}

void NodeSelection::reject() {

    QDialog::reject();
}

void NodeSelection::accept() {


    if ( selectedNode==-1){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No node selected</h2>"
                                                   "<p>Please choose a node!</p>"));

    } else{

        QDialog::accept();
    }

}

int NodeSelection::getNode(){

    return selectedNode;
}


