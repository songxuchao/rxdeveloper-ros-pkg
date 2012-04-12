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

    model_subscriptions= new QStandardItemModel(node->subscriptions.size(),3);
    model_subscriptions->setHeaderData(0,Qt::Horizontal,QObject::tr("topic"));
    model_subscriptions->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    model_subscriptions->setHeaderData(2,Qt::Horizontal,QObject::tr("comment"));
    QStringList subs;
    for(std::list<Topic_Type>::iterator iter=node->subscriptions.begin();iter != node->subscriptions.end();iter++ )
    {
        subs.push_back((QString("%1 %2 %3").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)).arg(QString::fromStdString(Topic_Type(*iter).topiccomment))));
    }
    for (int i=0;i<subs.count();i++){
        QStringList sub = subs.at(i).split(" ");
        QStandardItem *item0;
        item0 = new QStandardItem(sub.at(0));
        model_subscriptions->setItem(i,0,item0);
        item0 = new QStandardItem(sub.at(1));
        model_subscriptions->setItem(i,1,item0);
        item0 = new QStandardItem(sub.at(2));
        model_subscriptions->setItem(i,2,item0);
    }

    model_publications= new QStandardItemModel(node->publications.size(),3);
    model_publications->setHeaderData(0,Qt::Horizontal,QObject::tr("topic"));
    model_publications->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    model_publications->setHeaderData(2,Qt::Horizontal,QObject::tr("comment"));
    QStringList pubs;
    for(std::list<Topic_Type>::iterator iter=node->publications.begin();iter != node->publications.end();iter++ )
    {
        pubs.push_back((QString("%1 %2 %3").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)).arg(QString::fromStdString(Topic_Type(*iter).topiccomment))));
    }
    for (int i=0;i<pubs.count();i++){
        QStringList pub = pubs.at(i).split(" ");
        QStandardItem *item0;
        item0 = new QStandardItem(pub.at(0));
        model_publications->setItem(i,0,item0);
        item0 = new QStandardItem(pub.at(1));
        model_publications->setItem(i,1,item0);
        item0 = new QStandardItem(pub.at(2));
        model_publications->setItem(i,2,item0);
    }

    model_services= new QStandardItemModel(node->services.size(),3);
    model_services->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    model_services->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    model_services->setHeaderData(2,Qt::Horizontal,QObject::tr("comment"));
    QStringList servs;
    for(std::list<Topic_Type>::iterator iter=node->services.begin();iter != node->services.end();iter++ )
    {
        servs.push_back((QString("%1 %2 %3").arg(QString::fromStdString(Topic_Type(*iter).topic)).arg(QString::fromStdString(Topic_Type(*iter).topictype)).arg(QString::fromStdString(Topic_Type(*iter).topiccomment))));
    }
    for (int i=0;i<servs.count();i++){
        QStringList serv = servs.at(i).split(" ");
        QStandardItem *item0;

        item0 = new QStandardItem(serv.at(0));
        model_services->setItem(i,0,item0);
        item0 = new QStandardItem(serv.at(1));
        model_services->setItem(i,1,item0);
        item0 = new QStandardItem(serv.at(2));
        model_services->setItem(i,2,item0);
    }


    model_parameters= new QStandardItemModel(node->parameters.size(),5);
    model_parameters->setHeaderData(0,Qt::Horizontal,QObject::tr("name"));
    model_parameters->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    model_parameters->setHeaderData(2,Qt::Horizontal,QObject::tr("default"));
    model_parameters->setHeaderData(3,Qt::Horizontal,QObject::tr("range"));
    model_parameters->setHeaderData(4,Qt::Horizontal,QObject::tr("comment"));
    QStringList params;
    for(std::list<Name_Type_Default>::iterator iter=node->parameters.begin();iter != node->parameters.end();iter++ )
    {
        params.push_back((QString("%1 %2 %3 %4 %5").arg(QString::fromStdString(Name_Type_Default(*iter).paramName)).arg(QString::fromStdString(Name_Type_Default(*iter).paramType)).arg(QString::fromStdString(Name_Type_Default(*iter).paramDefault)).arg(QString::fromStdString(Name_Type_Default(*iter).paramRange)).arg(QString::fromStdString(Name_Type_Default(*iter).paramcomment))));
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
        item0 = new QStandardItem(param.at(3));
        model_parameters->setItem(i,3,item0);
        item0 = new QStandardItem(param.at(4));
        model_parameters->setItem(i,4,item0);
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
    ui->tableView_parameters->setColumnWidth(3,69);

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
            tempTT.topic=(model_subscriptions->item(i,0))->text().toStdString();
            tempTT.topictype=(model_subscriptions->item(i,1))->text().toStdString();
            tempTT.topiccomment=(model_subscriptions->item(i,2))->text().toStdString();
            mynode->subscriptions.push_back(tempTT);
        }
        mynode->publications.clear();
        for (int i=0;i<model_publications->rowCount();i++){

            tempTT.topic=(model_publications->item(i,0))->text().toStdString();
            tempTT.topictype=(model_publications->item(i,1))->text().toStdString();
            tempTT.topiccomment=(model_publications->item(i,2))->text().toStdString();
            mynode->publications.push_back(tempTT);
        }
        mynode->services.clear();
        for (int i=0;i<model_services->rowCount();i++){

            tempTT.topic=(model_services->item(i,0))->text().toStdString();
            tempTT.topictype=(model_services->item(i,1))->text().toStdString();
            tempTT.topiccomment=(model_services->item(i,2))->text().toStdString();
            mynode->services.push_back(tempTT);
        }
        mynode->parameters.clear();
        for (int i=0;i<model_parameters->rowCount();i++){
            tempNTD.paramName=(model_parameters->item(i,0))->text().toStdString();
            tempNTD.paramType=(model_parameters->item(i,1))->text().toStdString();
            tempNTD.paramDefault=(model_parameters->item(i,2))->text().toStdString();
            tempNTD.paramRange=(model_parameters->item(i,3))->text().toStdString();
            tempNTD.paramcomment=(model_parameters->item(i,4))->text().toStdString();
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
    model_subscriptions->setData(model_subscriptions->index(model_subscriptions->rowCount() - 1, 2), "");
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
    model_publications->setData(model_publications->index(model_publications->rowCount() - 1, 2), "");
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
    model_services->setData(model_services->index(model_services->rowCount() - 1, 2), "");
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
    model_parameters->setData(model_parameters->index(model_parameters->rowCount() - 1, 3), "");
    model_parameters->setData(model_parameters->index(model_parameters->rowCount() - 1, 4), "");
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
            text.append(QString("\t\t %1.desc= %2\n").arg(i).arg(model_subscriptions->item(i,2)->text()));
        }
        text.append("\t}");
    }
    if (model_publications->rowCount()>0){
        text.append("\n\tpub{\n");
        for (int i=0;i<model_publications->rowCount();i++){

            text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_publications->item(i,0)->text()));
            text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_publications->item(i,1)->text()));
            text.append(QString("\t\t %1.desc= %2\n").arg(i).arg(model_publications->item(i,2)->text()));
        }
        text.append("\t}");
    }
    if (model_services->rowCount()>0){
        text.append("\n\tsrv{\n");
        for (int i=0;i<model_services->rowCount();i++){

            text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_services->item(i,0)->text()));
            text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_services->item(i,1)->text()));
            text.append(QString("\t\t %1.desc= %2\n").arg(i).arg(model_services->item(i,2)->text()));
        }
        text.append("\t}");
    }
    if (model_parameters->rowCount()>0){
        text.append("\n\tparam{\n");
        for (int i=0;i<model_parameters->rowCount();i++){

            text.append(QString("\t\t %1.name= %2\n").arg(i).arg(model_parameters->item(i,0)->text()));
            text.append(QString("\t\t %1.default= %2\n").arg(i).arg(model_parameters->item(i,2)->text()));
            text.append(QString("\t\t %1.type= %2\n").arg(i).arg(model_parameters->item(i,1)->text()));
            text.append(QString("\t\t %1.desc= %2\n").arg(i).arg(model_parameters->item(i,3)->text()));
        }
        text.append("\t}");
    }

    text.append("\n}\n}}}");
    wiki->setText(text);
    wiki->exec();
}

/**
 * try to create a c++-template from the given information
 **/
void SpecFileEdit::on_pushButton_createCpp_clicked()
{

    QProcess getPackagePath;

    getPackagePath.start(QString("rospack find "+ui->lineEdit_package->text()));

    getPackagePath.waitForFinished(-1);
    QByteArray output = getPackagePath.readAllStandardOutput();
    QString packagePath = output.trimmed();
    QDir currentDir;
    if (output.startsWith("/")){
        currentDir.setPath(packagePath);
        QDir::setCurrent(currentDir.absolutePath());
        QFile test("test.tmp");
        bool writable = test.open(QIODevice::ReadWrite);
        test.close();
        if (writable){
            test.remove();
            currentDir.mkdir("src");
            packagePath.append("/src");
            currentDir.setPath(packagePath);
            QDir::setCurrent(currentDir.absolutePath());

            //Write the new File
            QFile newFile;
            newFile.setFileName(QString(ui->lineEdit_package->text()+".cpp"));
            bool reallyWriteFile=true;
            if (!(newFile.exists()))
            {
                reallyWriteFile=true;
            }else{
                switch( QMessageBox::question( this, tr("File already exists!"),
                                               tr("Do you want to overwrite the existing file?\nAre you sure?"),
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::No ))
                {
                case QMessageBox::Yes:
                    reallyWriteFile=true;
                    break;
                case QMessageBox::No:
                    reallyWriteFile=false;
                    break;
                default:
                    reallyWriteFile=false;
                    break;
                }
            }
            if(reallyWriteFile){
                newFile.open(QIODevice::ReadWrite);
                QTextStream outstream( &newFile );        // use a text stream

                QString header;
                header=QString( "// src/"+ui->lineEdit_type->text()+"\t created from specfile\n\n"+
                                "#include <ros/ros.h> // You must include this to do things with ROS.\n"+
                                "#include \"std_msgs/String.h\"\n\n"+
                                "int main (int argc, char** argv)\n"+
                                "{\n"+
                                "\tros::init(argc, argv, \""+ui->lineEdit_type->text()+"\");\n"+
                                "\tros::NodeHandle nh;\n");

                QString footer="";
                footer= QString("\n\tros::Rate loop_rate(10);\n\n\twhile (ros::ok())\n\t{\n");
                for (int i=0;i<model_publications->rowCount();i++){
                    QString type=model_publications->item(i,1)->text();
                    if(type.contains("std_msgs/")){
                        type.replace("std_msgs/","std_msgs::");
                    }
                    footer.append(QString("\t\tpub_%1.publish(msg);\t //create message msg of the right type: %2\n").arg(i).arg(type));
                }
                footer.append(QString("\t\tros::spinOnce();\n\t\tloop_rate.sleep();\n\t}\n\n\treturn 0;\n}\n"));

                QString pubs="";
                if (model_publications->rowCount()>0){
                    pubs="\n\t//publish on topics\n";
                    for (int i=0;i<model_publications->rowCount();i++){
                        QString type=model_publications->item(i,1)->text();
                        if(type.contains("std_msgs/")){
                            type.replace("std_msgs/","std_msgs::");
                        }
                        pubs.append(QString("\tros::Publisher pub_%1 = nh.advertise<%2>(\"%3\", 1000);\n").arg(i).arg(type).arg(model_publications->item(i,0)->text()));
                    }
                }

                QString subs="";
                if (model_subscriptions->rowCount()>0){
                    subs="\n\t//subscribe to topics\n";
                    for (int i=0;i<model_subscriptions->rowCount();i++){
                        QString type=model_subscriptions->item(i,1)->text();
                        if(type.contains("std_msgs/")){
                            type.replace("std_msgs/","std_msgs::");
                        }
                        subs.append(QString("\tros::Subscriber sub_%1 = nh.subscribe(\"%2\", 1000, sub%1_method);\t// please create the method: sub%1_method\n").arg(i).arg(model_subscriptions->item(i,0)->text()));
                    }
                }
                QString servs="";
                if (model_services->rowCount()>0){
                    servs="\n\t//services provided\n";
                    for (int i=0;i<model_services->rowCount();i++){
                        QString type=model_services->item(i,1)->text();
                        if(type.contains("std_msgs/")){
                            type.replace("std_msgs/","std_msgs::");
                        }
                        servs.append(QString("\tros::ServiceServer service_%1 = nh.advertiseService(\"%2\", service%1_method);\t// please create the method: service%1_method\n").arg(i).arg(model_services->item(i,0)->text()));
                    }
                }
                QString params="";
                if (model_parameters->rowCount()>0){
                    params="\n\t//parameters\n";
                    for (int i=0;i<model_parameters->rowCount();i++){
                        QString type=model_parameters->item(i,1)->text();
                        if(type.contains("std_msgs/")){
                            type.replace("std_msgs/","std_msgs::");
                        }
                        QString value=model_parameters->item(i,2)->text();
                        if(type.contains("string")){
                            if(!value.startsWith("\"")){
                                value.prepend("\"");
                            }
                            if(!value.endsWith("\"")){
                                value.append("\"");
                            }
                        }
                        params.append(QString("\t nh.setParam(\"%1\", %2);\t// Param of Type: %3\n").arg(model_parameters->item(i,0)->text()).arg(value).arg(type));
                    }
                }

                // write the output
                outstream << header.toUtf8().data() << pubs.toUtf8().data() << subs.toUtf8().data() << servs.toUtf8().data() << params.toUtf8().data() << footer.toUtf8().data();
                newFile.close();

            }


        }else
            QMessageBox::warning(this, (QString::fromUtf8("Error")),
                                 QString::fromUtf8("<h2>Folder is not writable!</h2>"
                                                   "<p>You don't have permission to create a file in: %1</p>").arg(packagePath));
    }
    else
        QMessageBox::warning(this, (QString::fromUtf8("Error")),
                             QString::fromUtf8("<h2>Package does not exist!</h2>"
                                               "<p>Please create the package \"%1\" before trying to create files!</p>").arg(ui->lineEdit_package->text()));



}

