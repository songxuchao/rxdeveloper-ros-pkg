#include "ui_specFileEdit.h"
#include "specFileEdit.h"
/*!\brief small dialog for naming.
 *
 * ...
 */
SpecFileEdit::SpecFileEdit(rosNode *node,QWidget *parent) : QDialog(parent), ui(new Ui::SpecFileEdit) {
    ui->setupUi(this);
    mynode=node;
    ui->lineEdit_type->setText(node->type);
    ui->lineEdit_package->setText(node->package);

    QStringList sublist;
    sublist<<"eins"<<"zwei";
    model_subscriptions = new QStringListModel();
    model_publications = new QStringListModel();
    model_services = new QStringListModel();
    model_parameters = new QStringListModel();

    model_subscriptions->setStringList(node->subscriptions);
    model_publications->setStringList(node->publications);
    model_services->setStringList(node->services);
    model_parameters->setStringList(node->parameters);

    ui->listView_subscriptions->setModel(model_subscriptions);
    ui->listView_publications->setModel(model_publications);
    ui->listView_services->setModel(model_services);
    ui->listView_parameters->setModel(model_parameters);

    ui->toolButton_addSub->setDefaultAction(ui->actionAdd_sub);
    ui->toolButton_addPub->setDefaultAction(ui->actionAdd_pub);
    ui->toolButton_addServ->setDefaultAction(ui->actionAdd_serv);
    ui->toolButton_addParam->setDefaultAction(ui->actionAdd_param);
    ui->toolButton_delSub->setDefaultAction(ui->actionDelete_sub);
    ui->toolButton_delPub->setDefaultAction(ui->actionDelete_pub);
    ui->toolButton_delServ->setDefaultAction(ui->actionDelete_serv);
    ui->toolButton_delParam->setDefaultAction(ui->actionDelete_param);

    connect(ui->listView_subscriptions,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_subs(const QModelIndex &)));
    connect(ui->listView_publications,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_pubs(const QModelIndex &)));
    connect(ui->listView_services,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_servs(const QModelIndex &)));
    connect(ui->listView_parameters,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_params(const QModelIndex &)));


}

SpecFileEdit::~SpecFileEdit() {
    delete ui;
}

void SpecFileEdit::reject() {
    QDialog::reject();
}

void SpecFileEdit::accept() {


    if (ui->lineEdit_type->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Name missing</h2>"
                                                   "<p>Please insert a correct name!</p>"));
        ui->lineEdit_type->setFocus();

    } else{
        mynode->type= ui->lineEdit_type->text();
        mynode->package = ui->lineEdit_package->text();
        mynode->subscriptions= model_subscriptions->stringList();
        mynode->publications= model_publications->stringList();
        mynode->services =model_services->stringList();
        mynode->parameters =model_parameters->stringList();
        QDialog::accept();
    }

}

void SpecFileEdit::selectionHandle_subs(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedSub=index.row();
    }
}
void SpecFileEdit::selectionHandle_pubs(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedPub=index.row();
    }
}
void SpecFileEdit::selectionHandle_servs(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedServ=index.row();
    }
}
void SpecFileEdit::selectionHandle_params(const QModelIndex &index)
{
    if (index.isValid()){   //prevents index out of bounds errors
        selectedParam=index.row();
    }
}

void SpecFileEdit::on_actionDelete_sub_triggered()
{
    model_subscriptions->removeRow(selectedSub);
}

void SpecFileEdit::on_actionAdd_sub_triggered()
{
    int row = model_subscriptions->rowCount();
    model_subscriptions->insertRow(row);
    model_subscriptions->setData(model_subscriptions->index(model_subscriptions->rowCount() - 1, 0), "<new>");
}

void SpecFileEdit::on_actionDelete_pub_triggered()
{
    model_publications->removeRow(selectedPub);
}

void SpecFileEdit::on_actionAdd_pub_triggered()
{
    int row = model_publications->rowCount();
    model_publications->insertRow(row);
    model_publications->setData(model_publications->index(model_publications->rowCount() - 1, 0), "<new>");
}
void SpecFileEdit::on_actionDelete_serv_triggered()
{
    model_services->removeRow(selectedServ);
}

void SpecFileEdit::on_actionAdd_serv_triggered()
{
    int row = model_services->rowCount();
    model_services->insertRow(row);
    model_services->setData(model_services->index(model_services->rowCount() - 1, 0), "<new>");
}
void SpecFileEdit::on_actionDelete_param_triggered()
{
    model_parameters->removeRow(selectedParam);
}

void SpecFileEdit::on_actionAdd_param_triggered()
{
    int row = model_parameters->rowCount();
    model_parameters->insertRow(row);
    model_parameters->setData(model_parameters->index(model_parameters->rowCount() - 1, 0), "<new>");
    //model_parameters->row(row)->doubleClicked();

}


