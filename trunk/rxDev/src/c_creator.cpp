#include "rxdev.h"
#include "ui_rxdev.h"
#include "newEntry.h"
#include "QCompleter"
#include <QUrl>
#include "specFileEdit.h"

QPoint menuPoint;
QString folderName;


void RxDev::enableCreatePackageButton(const QString &text)
{
    ui->pushButton_createPackage->setEnabled(!text.isEmpty());
}


void RxDev::setupCreator(){


    workingModel = new QFileSystemModel;
    workingModel->setRootPath(workingDir.absolutePath());
    ui->treeView_packageBrowser->setModel(workingModel);
    ui->treeView_packageBrowser->setRootIndex(workingModel->index(workingDir.absolutePath()));

    ui->treeView_packageBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView_packageBrowser, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint&)));
    connect(ui->treeView_packageBrowser->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this, SLOT(selectionHandle_3(const QItemSelection &,const QItemSelection &)));
    //connect(ui->treeView_packageBrowser, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(openFileOrFolder(const QModelIndex&)));
    ui->treeView_packageBrowser->setColumnWidth(0,600);

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

void RxDev::showContextMenu(const QPoint&point){
    QMenu contextMenu;

    QModelIndex index(ui->treeView_packageBrowser->indexAt(point));
    menuPoint = point;
    if(workingModel->fileInfo(index).isDir()){
        contextMenu.addAction(tr("new File"),this, SLOT(createNewFile()));
        contextMenu.addAction(tr("new SpecFile"),this, SLOT(createNewSpecFile()));
        contextMenu.addAction(tr("new C++-NodeletFile"),this, SLOT(createNewCpp_NodeletFile()));
        contextMenu.addAction(tr("new Python-NodeletFile"),this, SLOT(createNewPython_NodeletFile()));
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
        QDesktopServices::openUrl(QUrl::fromLocalFile( filePath));
    }
    else if(workingModel->fileInfo(index).isDir())
    {
        QString folderPath =  workingModel->fileInfo(index).absolutePath()+"/"+folderName;
        QDesktopServices::openUrl(QUrl::fromLocalFile( folderPath));
    }
}
void RxDev::createNewFolder(){
    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;

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

    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
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


void RxDev::createNewSpecFile()
{
    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
    if (!folderPath.endsWith("/node"))
        QMessageBox::warning( this, "No correct specfilefolder!",
                             "If you continue, rxDeveloper will not be able to use the specfile.\nSpecfiles have to be in '<packagename>/node/'-folder structure");

    NewEntry createFile;
    createFile.setWindowTitle("Create File in "+folderPath);
    createFile.exec();
    QString filename=createFile.getFileName();
    QFile newFile;
    if (!(filename=="")){
        if (!filename.endsWith(".node"))
            filename.append(".node");
        newFile.setFileName(filename);
    }
    QString filePath= folderPath.append("/"+newFile.fileName());
    rosNode newSpec;
    SpecFileEdit specFile(&newSpec);
    specFile.setWindowTitle("Specfile: "+createFile.getFileName());
    bool accept = specFile.exec();
    if ((accept)){
        writeSpecFile(&newSpec,filePath);

    }

}

void RxDev::createNewCpp_NodeletFile()
{
    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
    NewEntry createFile;
    createFile.setWindowTitle("Create File in "+folderPath);
    createFile.exec();
    QString filename=createFile.getFileName();
    QFile newFile;
    if (!(filename=="")){
        if (!filename.endsWith(".cpp")&&!filename.endsWith(".c")&&!filename.endsWith(".cc")&&!filename.endsWith(".c++"))
            filename.append(".cpp");
        newFile.setFileName(filename);
        QDir::setCurrent(folderPath);
        if (!(newFile.exists()))
        {
            newFile.open(QIODevice::ReadWrite);
            newFile.close();
        }
    }

}

void RxDev::createNewPython_NodeletFile()
{
    QString folderPath =  workingModel->fileInfo(ui->treeView_packageBrowser->indexAt(menuPoint)).absolutePath()+"/"+folderName;
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


//Selection handler for the columnView at the component creator-tab
void RxDev::selectionHandle_3(const QItemSelection &selected,const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if (!selected.isEmpty()){   //prevents index out of bounds errors
        folderName = selected.indexes()[0].data().toString();
    }
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
    ui->lineEdit_packageName->setText("");
}
