#include "rxdev.h"
#include "ui_rxdev.h"
#include "newEntry.h"
#include "QCompleter"
#include <QUrl>
#include "specFileEdit.h"
#include "specFileParser.h"
#include "nodeSelection.h"


QPoint menuPoint;
QString folderName;


void RxDev::enableCreatePackageButton(const QString &text)
{
    ui->pushButton_createPackage->setEnabled(!text.isEmpty());
}

void RxDev::refresh_packageModel(){
    QStringList workingDirPackages;
    for(int pack=0; pack<packageList.count();pack++){
        //Get package-path
        QProcess getPackagePath;
        getPackagePath.setWorkingDirectory(workingDir.absolutePath());
        getPackagePath.start(QString("rospack find "+packageList[pack]));
        getPackagePath.waitForFinished(-1);
        QByteArray output = getPackagePath.readAllStandardOutput();
        QString packagePath = output.trimmed();
        if (packagePath.startsWith(workingDir.absolutePath()))
            workingDirPackages<<packageList.at(pack);
    }
    packageModel->setStringList(workingDirPackages);
}

void RxDev::setupCreator(){


    packageModel = new QStringListModel();
    ui->listView_package->setModel(packageModel);
    refresh_packageModel();
    connect(ui->listView_package->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this, SLOT(selectionHandle_packages(const QItemSelection &,const QItemSelection &)));

    workingModel = new QFileSystemModel;
    workingModel->setRootPath(workingDir.absolutePath());
    ui->treeView_packageBrowser->setModel(workingModel);

    ui->treeView_packageBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView_packageBrowser, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint&)));
    connect(ui->treeView_packageBrowser->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this, SLOT(selectionHandle_selectedPackage(const QItemSelection &,const QItemSelection &)));
    //connect(ui->treeView_packageBrowser, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(openFileOrFolder(const QModelIndex&)));
    ui->treeView_packageBrowser->setColumnWidth(0,300);

    QRegExp regExp_packageName("[a-z]+[a-zA-Z0-9]{,20}_?[a-zA-Z_0-9]*");           //conventions
    QRegExp regExp_packageDependencies("[a-zA-Z _0-9]*");
    ui->lineEdit_packageDependencies->setValidator(new QRegExpValidator(regExp_packageDependencies, this));
    ui->lineEdit_packageName->setValidator(new QRegExpValidator(regExp_packageName, this));
    connect(ui->lineEdit_packageName, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableCreatePackageButton(const QString &)));

    QCompleter *completer = new QCompleter(packageList);
    completer->setCaseSensitivity(Qt::CaseInsensitive); //Make caseInsensitive selection
    //completer->setCompletionMode(QCompleter::InlineCompletion); //Used to enable in line searching
    completer->setCompletionPrefix ( ui->lineEdit_packageDependencies->text().split(" ").back().trimmed().toLower() );
    ui->lineEdit_packageDependencies->setCompleter(completer);
}

void RxDev::selectionHandle_packages(const QItemSelection &selected, const QItemSelection &)
{


    QString pack= selected.indexes()[0].data().toString();
    QProcess getPackagePath;
    getPackagePath.setWorkingDirectory(workingDir.absolutePath());
    getPackagePath.start(QString("rospack find "+pack));
    getPackagePath.waitForFinished(-1);
    QByteArray output = getPackagePath.readAllStandardOutput();
    packagePath = output.trimmed();
    folderPath=packagePath;
    bool isPack = packagePath.startsWith("/");
    ui->pushButton_createCpp->setEnabled(isPack);
    ui->pushButton_createPython->setEnabled(isPack);
    ui->pushButton_specfile->setEnabled(isPack);
    ui->pushButton_specfilFromExe->setEnabled(isPack);

    if (isPack){
        //qDebug()<<packagePath;
        workingModel->setRootPath(packagePath);

        ui->treeView_packageBrowser->setRootIndex(workingModel->index(packagePath));

        ui->groupBox_package->setTitle("package: "+pack);
    }
}
//Selection handler for the columnView at the component creator-tab
void RxDev::selectionHandle_selectedPackage(const QItemSelection &selected,const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if (!selected.isEmpty()){   //prevents index out of bounds errors

        folderName = selected.indexes()[0].data().toString();
        folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->currentIndex()).absolutePath()+"/"+folderName;
        //qDebug() << folderPath;
        //folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->.absolutePath()+"/"+folderName;


    }
}

void RxDev::showContextMenu(const QPoint&point){
    QMenu contextMenu;

    QModelIndex index(ui->treeView_packageBrowser->indexAt(point));
    menuPoint = point;
    //        qDebug()<<index;
    if(workingModel->fileInfo(index).isDir()||!index.isValid()){
        contextMenu.addAction(tr("new File"),this, SLOT(createNewFile()));
        contextMenu.addAction(tr("new SpecFile"),this, SLOT(on_pushButton_specfile_clicked()));
        contextMenu.addAction(tr("new SpecFile from running process"),this, SLOT(on_pushButton_specfilFromExe_clicked()));
        contextMenu.addAction(tr("new C++-File"),this, SLOT(on_pushButton_createCpp_clicked()));
        contextMenu.addAction(tr("new Python-File"),this, SLOT(on_pushButton_createPython_clicked()));
        contextMenu.addSeparator();
        contextMenu.addAction(tr("new Folder"),this, SLOT(createNewFolder()));
        contextMenu.addAction(tr("browse Folder"),this, SLOT(openFileOrFolder()));
        contextMenu.addAction(tr("rename Folder"),this, SLOT(renameFolder()));

    } else if(workingModel->fileInfo(index).isFile()){

        contextMenu.addAction(tr("open File"),this, SLOT(openFileOrFolder()));
        contextMenu.addSeparator();
        contextMenu.addAction(tr("delete File"),this, SLOT(deleteFile()));
    }
    contextMenu.exec(ui->treeView_packageBrowser->viewport()->mapToGlobal(point));
}

void RxDev::openFileOrFolder(){
    QModelIndex index(ui->treeView_packageBrowser->indexAt(menuPoint));
    if(workingModel->fileInfo(index).isFile())
    {
        QString filePath =  workingModel->filePath(index);
        if (filePath.endsWith(".node")){
            SpecFileParser *specParser = new SpecFileParser;
            specParser->nodeParser(filePath);

            SpecFileEdit specFile(&specParser->node);
            //qDebug()<<QString::fromStdString(specParser->node.type);
            specFile.setWindowTitle("Specfile: "+filePath);
            bool accept = specFile.exec();
            if ((accept)){
                QFile file;
                file.setFileName(filePath);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                if (file.isWritable()){
                    QString tempContens = specParser->writeSpecFile(&specParser->node);
                    QTextStream text(&file);
                    text<<tempContens;
                    //qDebug()<<"subs "<<specFile.getSubscriptions();
                    file.close();
                    QMessageBox::information( this, "File written!", "The file "+filePath+" was updated\n", QMessageBox::Ok, 0 );
                    on_pushButton_refreshNodesOrComps_clicked();
                } else
                    QMessageBox::critical(this, "File is write protected!", "The file "+filePath+" could not get updated\n", QMessageBox::Ok, 0 );

                //        setType(nodeEdit.getType());
                //        setName(nodeEdit.getName());
                //        setArgs(nodeEdit.getArgs());


            }
        }else
            QDesktopServices::openUrl(QUrl::fromLocalFile( filePath));
    }
    else// if(workingModel->fileInfo(index).isDir())
    {
        QString folderPath;


        if (index.isValid())
            folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
        else
            folderPath= packagePath;

        QDesktopServices::openUrl(QUrl::fromLocalFile( folderPath));
    }
}
void RxDev::createNewFolder(){
    QString folderPath;
    QModelIndex index(ui->treeView_packageBrowser->indexAt(menuPoint));

    if (index.isValid())
        folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
    else
        folderPath= packagePath;
    NewEntry createFolder;
    createFolder.setWindowTitle("Create Folder in "+folderPath);
    createFolder.exec();
    QDir newDir;
    if (!(createFolder.getFileName()=="")){
        QDir::setCurrent(folderPath);
        newDir.mkdir(createFolder.getFileName());
    }
}


void RxDev::createNewFile()
{
    QString folderPath;
    QModelIndex index(ui->treeView_packageBrowser->indexAt(menuPoint));

    if (index.isValid())
        folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
    else
        folderPath= packagePath;

    NewEntry createFile;
    createFile.setWindowTitle("Create File in "+folderPath);
    createFile.exec();

    QFile newFile;
    if (!(createFile.getFileName()=="")){
        newFile.setFileName(createFile.getFileName());
        QDir::setCurrent(folderPath);
        if (!(newFile.exists()))
        {
            newFile.open(QIODevice::ReadWrite);
            newFile.close();
        }
    }

}




void RxDev::renameFolder()
{
    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
    NewEntry rename;
    rename.setWindowTitle(("Rename: "+folderPath));
    rename.exec();

    QDir current;
    //Create new File
    if (!(rename.getFileName()==""))
        current.rename(folderPath,(workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+rename.getFileName()));

}




void RxDev::deleteFile(){

    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/";
    QFile delFile;
    delFile.setFileName(folderName);
    QDir::setCurrent(folderPath);
    delFile.remove(folderName);

}



void RxDev::on_pushButton_createPackage_clicked()
{
    QProcess rosCreatePackage;
    rosCreatePackage.setWorkingDirectory(workingDir.absolutePath());
    rosCreatePackage.setProcessChannelMode(QProcess::MergedChannels);
    rosCreatePackage.start(QString("roscreate-pkg "+ui->lineEdit_packageName->text()+" "+ui->lineEdit_packageDependencies->text()));
    rosCreatePackage.waitForFinished(-1);
    packageModel->insertRow(0);
    packageModel->setData(packageModel->index(0, 0), ui->lineEdit_packageName->text());
    ui->lineEdit_packageName->setText("");
}
void RxDev::on_pushButton_createCpp_clicked()
{
    if (!packagePath.endsWith("/src")){
        currentDir.setPath(packagePath);
        currentDir.mkdir("src");
        packagePath.append("/src");
    }
    NewEntry createFile;
    createFile.setWindowTitle("Create File in "+folderPath);
    createFile.exec();
    QString filename=createFile.getFileName();
    QFile newFile;
    if (!(filename=="")){
        if (!filename.endsWith(".cpp")&&!filename.endsWith(".c")&&!filename.endsWith(".C")&&!filename.endsWith(".h")&&!filename.endsWith(".hpp")&&!filename.endsWith(".h++")&&!filename.endsWith(".cc")&&!filename.endsWith(".c++"))
            filename.append(".cpp");
        newFile.setFileName(filename);
        QDir::setCurrent(folderPath);
        if (!(newFile.exists()))
        {
            newFile.open(QIODevice::WriteOnly | QIODevice::Text);
            newFile.close();
        }
        QDir::setCurrent(packagePath);
    }
}

void RxDev::on_pushButton_createPython_clicked()
{
    if (!packagePath.endsWith("/src")){
        currentDir.setPath(packagePath);
        currentDir.mkdir("src");
    }
    NewEntry createFile;
    createFile.setWindowTitle("Create File in "+folderPath);
    createFile.exec();
    QString filename=createFile.getFileName();
    QFile newFile;
    if (!(filename=="")){
        if (!filename.endsWith(".pyd")&&!filename.endsWith(".pyo")&&!filename.endsWith(".py"))
            filename.append(".py");
        newFile.setFileName(filename);
        QDir::setCurrent(folderPath);
        if (!(newFile.exists()))
        {
            newFile.open(QIODevice::ReadWrite);
            newFile.close();
        }
        QDir::setCurrent(packagePath);
    }

}

void RxDev::on_pushButton_specfile_clicked()
{
    qDebug()<<packagePath;
    currentDir.setPath(workingModel->rootPath());
    currentDir.mkdir("node");
    NewEntry createFile;
    createFile.setWindowTitle("Create File in "+workingModel->rootPath()+("/node"));
    bool name = createFile.exec();
    if (name)
    {
        QString filename=createFile.getFileName();
        QFile newFile;
        if (!(filename=="")){
            if (!filename.endsWith(".node"))
                filename.append(".node");
            newFile.setFileName(filename);
        }
        QString filePath= workingModel->rootPath()+("/node/"+newFile.fileName());

        Specfile newSpec;
        SpecFileEdit specFile(&newSpec);
        specFile.setWindowTitle("Specfile: "+createFile.getFileName());

        bool accept = specFile.exec();
        SpecFileParser *specParser = new SpecFileParser;
        if ((accept)){
            QFile file;
            file.setFileName(filePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            if (file.isWritable()){
                QString tempContens = specParser->writeSpecFile(&newSpec);
                QTextStream text(&file);
                text<<tempContens;

                //qDebug()<<"subs "<<specFile.getSubscriptions();
                file.close();
                QMessageBox::information( this, "File written!", "The file "+filePath+" was updated\n", QMessageBox::Ok, 0 );
                on_pushButton_refreshNodesOrComps_clicked();

            } else
                QMessageBox::critical(this, "File is write protected!", "The file "+filePath+" could not get updated\n", QMessageBox::Ok, 0 );

            //        setType(nodeEdit.getType());
            //        setName(nodeEdit.getName());
            //        setArgs(nodeEdit.getArgs());


        }

    }
    QDir::setCurrent(workingModel->rootPath());
}


void RxDev::on_pushButton_specfilFromExe_clicked()
{
    //fetch possible Nodes
    rosnode->setWorkingDirectory(QDir::temp().absolutePath());
    rosnode->setProcessChannelMode(QProcess::MergedChannels);
    rosnode->start(QString("rosnode list"));
    rosnode->waitForFinished(-1);
    QByteArray output = rosnode->readAll();
    if (output.contains("ERROR")){          //no nodes are running
        QMessageBox::information(this, QString::fromUtf8("There are no running ROS processes."),
                                QString::fromUtf8("Please run a ROS process and try again!"));
    }
    else{
        QStringList nodes;
        nodes.append(QString(output).split("\n"));
        nodes.removeLast();                     //removes an empty String at the end
        //let user choose from the available nodes
        NodeSelection nodeSelection(nodes);
        bool accept = nodeSelection.exec();
        if (accept){
            QString selected = nodes.at(nodeSelection.getNode());
            //qDebug()<<"==>"<<selected;

            // fetch information for selected node
            rosnode->start(QString("rosnode info "+selected));
            rosnode->waitForFinished(-1);
            QByteArray output = rosnode->readAll();
            //qDebug()<<output;
            QStringList info;
            Specfile newSpec;
            //selected.remove("/");
            //selected.toStdString() >> newSpec.type;
            info.append(QString(output).split(":"));
            QStringList pubs;
            pubs.append(QString(info[1]).split("\n"));
            for (int row=0;row<pubs.count();row++){
                if (QString(pubs[row]).startsWith(" * ")){
                    //qDebug()<<"Pubs: "<<pubs[row];
                    QString pub=pubs[row];
                    QStringList publist;
                    publist.append(pub.split(" ["));
                    Topic_Type tt;
                    tt.topic=publist[0].remove(" * ").toStdString();
                    tt.topictype= publist[1].remove("]").toStdString();
                    newSpec.publications.push_back(tt);
                }
            }
            QStringList subs;
            subs.append(QString(info[2]).split("\n"));
            for (int row=0;row<subs.count();row++){
                if (QString(subs[row]).startsWith(" * ")){
                    //qDebug()<<"Subs: "<<subs[row];
                    QString sub=subs[row];
                    QStringList sublist;
                    sublist.append(sub.split(" ["));
                    Topic_Type tt;
                    tt.topic=sublist[0].remove(" * ").toStdString();
                    tt.topictype= sublist[1].remove("]").toStdString();
                    newSpec.subscriptions.push_back(tt);
                }

            }
            QStringList servs;
            servs.append(QString(info[3]).split("\n"));
            for (int row=0;row<servs.count();row++){
                if (QString(servs[row]).startsWith(" * ")){
                    //qDebug()<<"servs: "<<servs[row];
                    QString serv=servs[row];
                    Topic_Type tt;
                    serv.remove(" * ");
                    tt.topic=serv.toStdString();
                    newSpec.services.push_back(tt);
                }

            }

            //qDebug()<<packagePath;
            currentDir.setPath(workingModel->rootPath());
            currentDir.mkdir("node");
            NewEntry createFile;
            createFile.setWindowTitle("Create File in "+workingModel->rootPath()+("/node"));
            bool name = createFile.exec();
            if (name)
            {
                QString filename=createFile.getFileName();
                QFile newFile;
                if (!(filename=="")){
                    if (!filename.endsWith(".node"))
                        filename.append(".node");
                    newFile.setFileName(filename);
                }
                QString filePath= workingModel->rootPath()+("/node/"+newFile.fileName());

                SpecFileEdit specFile(&newSpec);
                specFile.setWindowTitle("Specfile: "+createFile.getFileName());

                bool accept = specFile.exec();
                SpecFileParser *specParser = new SpecFileParser;
                if ((accept)){
                    QFile file;
                    file.setFileName(filePath);
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    if (file.isWritable()){
                        QString tempContens = specParser->writeSpecFile(&newSpec);
                        QTextStream text(&file);
                        text<<tempContens;

                        //qDebug()<<"subs "<<specFile.getSubscriptions();
                        file.close();
                        QMessageBox::information( this, "File written!", "The file "+filePath+" was created\n", QMessageBox::Ok, 0 );
                        on_pushButton_refreshNodesOrComps_clicked();

                    } else
                        QMessageBox::critical(this, "File is write protected!", "The file "+filePath+" could not be written\n", QMessageBox::Ok, 0 );

                    //        setType(nodeEdit.getType());
                    //        setName(nodeEdit.getName());
                    //        setArgs(nodeEdit.getArgs());


                }

            }
            QDir::setCurrent(workingModel->rootPath());

        }
    }

}
