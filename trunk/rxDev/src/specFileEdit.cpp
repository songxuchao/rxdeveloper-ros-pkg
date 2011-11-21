#include "ui_specFileEdit.h"
#include "specFileEdit.h"
#include "wikiDialog.h"
#include <QTextEdit>

/*!\brief small dialog for naming.
 *
 * ...
 */
SpecFileEdit::SpecFileEdit(Specfile *node,QWidget *parent) : QDialog(parent), ui(new Ui::SpecFileEdit) {
    ui->setupUi(this);
    mynode=node;
    ui->lineEdit_type->setText(QString::fromStdString(node->type));
    ui->lineEdit_package->setText(QString::fromStdString(node->package));

    model_subscriptions= new QStandardItemModel(node->subscriptions.size(),2);
    model_subscriptions->setHeaderData(0,Qt::Horizontal,QObject::tr("topic"));
    model_subscriptions->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    QStringList subs;
    for(std::list<Topic_Type>::iterator iter=node->subscriptions.begin();iter != node->subscriptions.end();iter++ )
    {
        subs.push_back((QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype))));
    }
    for (int i=0;i<subs.count();i++){
        QStringList sub = subs.at(i).split(" ");
        QStandardItem *item0;
         item0 = new QStandardItem(sub.at(0));
         model_subscriptions->setItem(i,0,item0);
         item0 = new QStandardItem(sub.at(1));
         model_subscriptions->setItem(i,1,item0);
    }

    model_publications= new QStandardItemModel(node->publications.size(),2);
    model_publications->setHeaderData(0,Qt::Horizontal,QObject::tr("topic"));
    model_publications->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    QStringList pubs;
    for(std::list<Topic_Type>::iterator iter=node->publications.begin();iter != node->publications.end();iter++ )
    {
        pubs.push_back((QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype))));
    }
    for (int i=0;i<pubs.count();i++){
        QStringList pub = pubs.at(i).split(" ");
        QStandardItem *item0;
         item0 = new QStandardItem(pub.at(0));
         model_publications->setItem(i,0,item0);
         item0 = new QStandardItem(pub.at(1));
         model_publications->setItem(i,1,item0);
    }

    model_services= new QStandardItemModel(node->publications.size(),2);
    model_services->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    model_services->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    QStringList servs;
    for(std::list<Topic_Type>::iterator iter=node->services.begin();iter != node->services.end();iter++ )
    {
        servs.push_back((QString("%1 %2").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype))));
    }
    for (int i=0;i<servs.count();i++){
        QStringList serv = servs.at(i).split(" ");
        QStandardItem *item0;

         item0 = new QStandardItem(serv.at(0));
         model_services->setItem(i,0,item0);
         item0 = new QStandardItem(serv.at(1));
         model_services->setItem(i,1,item0);
    }


    model_parameters= new QStandardItemModel(node->parameters.size(),3);
    model_parameters->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    model_parameters->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    model_parameters->setHeaderData(2,Qt::Horizontal,QObject::tr("default"));
    QStringList params;
    for(std::list<Name_Type_Default>::iterator iter=node->parameters.begin();iter != node->parameters.end();iter++ )
    {
        params.push_back((QString("%1 %2 %3").arg(QString::fromStdString(Name_Type_Default(*iter).paramName)).arg(QString::fromStdString(Name_Type_Default(*iter).paramType)).arg(QString::fromStdString(Name_Type_Default(*iter).paramDefault))));
        //todo add topictype anyhow
    }
    for (int i=0;i<params.count();i++){
        QStringList param = params.at(i).split(" ");
        QStandardItem *item0;

         item0 = new QStandardItem(param.at(0));
         model_parameters->setItem(i,0,item0);
         item0 = new QStandardItem(param.at(1));
         model_parameters->setItem(i,1,item0);
         item0 = new QStandardItem(param.at(2));
         model_parameters->setItem(i,2,item0);
    }





    ui->tableView_subscriptions->setModel(model_subscriptions);
    ui->tableView_subscriptions->setColumnWidth(0,180);
    ui->tableView_subscriptions->setColumnWidth(1,179);
    ui->tableView_publications->setModel(model_publications);
    ui->tableView_publications->setColumnWidth(0,180);
    ui->tableView_publications->setColumnWidth(1,179);

    ui->tableView_services->setModel(model_services);
    ui->tableView_services->setColumnWidth(0,180);
    ui->tableView_services->setColumnWidth(1,179);

    ui->tableView_parameters->setModel(model_parameters);
    ui->tableView_parameters->setColumnWidth(0,149);
    ui->tableView_parameters->setColumnWidth(1,149);
    ui->tableView_parameters->setColumnWidth(2,61);

    ui->toolButton_addSub->setDefaultAction(ui->actionAdd_sub);
    ui->toolButton_addPub->setDefaultAction(ui->actionAdd_pub);
    ui->toolButton_addServ->setDefaultAction(ui->actionAdd_serv);
    ui->toolButton_addParam->setDefaultAction(ui->actionAdd_param);
    ui->toolButton_delSub->setDefaultAction(ui->actionDelete_sub);
    ui->toolButton_delPub->setDefaultAction(ui->actionDelete_pub);
    ui->toolButton_delServ->setDefaultAction(ui->actionDelete_serv);
    ui->toolButton_delParam->setDefaultAction(ui->actionDelete_param);

    connect(ui->tableView_subscriptions,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_subs(const QModelIndex &)));
    connect(ui->tableView_publications,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_pubs(const QModelIndex &)));
    connect(ui->tableView_services,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_servs(const QModelIndex &)));
    connect(ui->tableView_parameters,SIGNAL(clicked(QModelIndex)), this, SLOT(selectionHandle_params(const QModelIndex &)));


}

SpecFileEdit::~SpecFileEdit() {
    delete ui;
}

void SpecFileEdit::reject() {
    QDialog::reject();
}

void SpecFileEdit::accept() {


    if (ui->lineEdit_type->text()==""||ui->lineEdit_package->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Type or package missing</h2>"
                                                   "<p>Please insert type and package data!</p>"));
        ui->lineEdit_type->setFocus();

    } else{
        mynode->type= ui->lineEdit_type->text().toStdString();
        mynode->package = ui->lineEdit_package->text().toStdString();
        Topic_Type tempTT;
        Name_Type_Default tempNTD;

        mynode->subscriptions.clear();
        for (int i=0;i<model_subscriptions->rowCount();i++){
            if (model_subscriptions->item(i,0)->text().isEmpty())
                tempTT.topic="";
            else
                tempTT.topic=(model_subscriptions->item(i,0))->text().toStdString();
            if (model_subscriptions->item(i,1)->text().isNull())
                tempTT.topictype="";
            else
                tempTT.topictype=(model_subscriptions->item(i,1))->text().toStdString();
            mynode->subscriptions.push_back(tempTT);
        }
        mynode->publications.clear();
        for (int i=0;i<model_publications->rowCount();i++){

                tempTT.topic=(model_publications->item(i,0))->text().toStdString();
                tempTT.topictype=(model_publications->item(i,1))->text().toStdString();
            mynode->publications.push_back(tempTT);
        }
        mynode->services.clear();
        for (int i=0;i<model_services->rowCount();i++){

                tempTT.topic=(model_services->item(i,0))->text().toStdString();

                tempTT.topictype=(model_services->item(i,1))->text().toStdString();
            mynode->services.push_back(tempTT);
        }
        mynode->parameters.clear();
        for (int i=0;i<model_parameters->rowCount();i++){
                tempNTD.paramName=(model_parameters->item(i,0))->text().toStdString();
                tempNTD.paramType=(model_parameters->item(i,1))->text().toStdString();
               tempNTD.paramDefault=(model_parameters->item(i,2))->text().toStdString();
            mynode->parameters.push_back(tempNTD);
        }


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
    model_subscriptions->setData(model_subscriptions->index(model_subscriptions->rowCount() - 1, 1), "");
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
    model_publications->setData(model_publications->index(model_publications->rowCount() - 1, 1), "");
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
    model_services->setData(model_services->index(model_services->rowCount() - 1, 1), "");
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
    model_parameters->setData(model_parameters->index(model_parameters->rowCount() - 1, 1), "");
    model_parameters->setData(model_parameters->index(model_parameters->rowCount() - 1, 2), "");
    //model_parameters->row(row)->doubleClicked();

}



void SpecFileEdit::on_pushButton_wiki_clicked()
{
    WikiDialog *wiki = new WikiDialog;

    QString text;
    text = QString("== Nodes ==\n{{{\n#!clearsilver CS/NodeAPI\nnode.0 {\n\tname = %1\n\tdesc = `%1` provides ...\n").arg(QString::fromStdString(mynode->type));
    if (model_subscriptions->rowCount()>0){
    text.append("\n\tsub{\n");
    for (int i=0;i<model_subscriptions->rowCount();i++){

        text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_subscriptions->item(i,0)->text()));
        text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_subscriptions->item(i,1)->text()));
        text.append(QString("\t\t %1.desc= \n").arg(i));
    }
    text.append("\t}");
    }
    if (model_publications->rowCount()>0){
    text.append("\n\tpub{\n");
    for (int i=0;i<model_publications->rowCount();i++){

        text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_publications->item(i,0)->text()));
        text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_publications->item(i,1)->text()));
        text.append(QString("\t\t %1.desc= \n").arg(i));
    }
    text.append("\t}");
    }
    if (model_services->rowCount()>0){
    text.append("\n\tsrv{\n");
    for (int i=0;i<model_services->rowCount();i++){

        text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_services->item(i,0)->text()));
        text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_services->item(i,1)->text()));
        text.append(QString("\t\t %1.desc= \n").arg(i));
    }
    text.append("\t}");
    }
    if (model_parameters->rowCount()>0){
    text.append("\n\tparam{\n");
    for (int i=0;i<model_parameters->rowCount();i++){

        text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_parameters->item(i,0)->text()));
        text.append(QString("\t\t %1.default= %2\n").arg(i).arg(model_parameters->item(i,2)->text()));
        text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_parameters->item(i,1)->text()));
        text.append(QString("\t\t %1.desc= \n").arg(i));
    }
    text.append("\t}");
    }

    text.append("\n}\n}}}");
    wiki->setText(text);
    wiki->exec();
}
