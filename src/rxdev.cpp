#include "rxdev.h"
#include "ui_rxdev.h"
#include <yaml-cpp/yaml.h>
#include <QCloseEvent>
#include <QComboBox>
#include <QToolBar>
#include <QUrl>
#include <QDateTime>
#include "launchFileWriter.h"
#include "launchFileReader.h"
#include "settings.h"



/*!\brief initialize GUI
 *
 * Starts the GUI and collects all important data.
 */
RxDev::RxDev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RxDev)
{
    //initialize UI
    ui->setupUi(this);

    //load Settings
    loadSettings();

    //initialize connector tab
    setupConnector();

    //initialize list of available nodes and adding data
    availableNodesOrComps();


    ui->dockWidget_errors->hide();
    //create Toolbar
    setupToolBar();

    //get TinyXML-Version
    tiny_version=(TIXML_MINOR_VERSION);


    ui->statusBar->showMessage(tr("Ready"));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(changeToolBar()));

    //initialize the creator tab
    setupCreator();

    setUnifiedTitleAndToolBarOnMac(true);

    rxgraph = new QProcess(this);
    connect(rxgraph, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(state( QProcess::ProcessState) ));
    rviz = new QProcess(this);
    connect(rviz, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(state( QProcess::ProcessState) ));
    rxloggerlevel = new QProcess(this);
    connect(rxloggerlevel, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(state( QProcess::ProcessState) ));
    rxconsole = new QProcess(this);
    connect(rxconsole, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(state( QProcess::ProcessState) ));
    dynamicreconfigure = new QProcess(this);
    connect(dynamicreconfigure, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(state( QProcess::ProcessState) ));
    roswtf = new QProcess(this);
    rosnode = new QProcess(this);
    rosLaunch = new QProcess(this);
    connect(rosLaunch, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(state( QProcess::ProcessState) ));
    if (rosLaunch->state()!=2){
        ui->actionStart->setEnabled(true);
        ui->actionStop->setEnabled(false);
    }else{
        ui->actionStart->setEnabled(false);
        ui->actionStop->setEnabled(true);
    }


    ui->tabWidget->setCurrentIndex(0);  //0 = connector tab
    ui->dockWidget_availableNodes->show();
    ui->dockWidget_availablePackages->hide();
}

RxDev::~RxDev(){
    ros::shutdown();
    delete ui;
}

/*!\brief save window information
 *
 * If the GUI is closed, save information about the window state.
 */
void RxDev::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        saveWindowState();
        event->accept();
    }
    else {
        event->ignore();
    }
}

/*!\brief resize window
 *
 * If the GUI is not launched for the first time or was not maximized in the last session,
 * restore the last window size.
 */
void RxDev::resizeEvent(QResizeEvent *event)
{
    if (!isMaximized())
        m_lastSize = size();
    QWidget::resizeEvent(event);
}

/*!\brief move window
 *
 * If the GUI is not launched for the first time or was not maximized in the last session,
 * restore the last window position.
 */
void RxDev::moveEvent(QMoveEvent *event)
{
    if (!isMaximized())
        m_lastPos = pos();
    QWidget::moveEvent(event);
}

//void RxDev::showEvent(QShowEvent *event)
//{
////    m_lastPos = pos();
////    m_lastSize = size();
////    QWidget::showEvent(event);
//}

void RxDev::restoreWindowState()
{
    QSettings settings;
    restoreState(settings.value("windowState").toByteArray());
    QPoint pos = settings.value("windowPos").toPoint();
    if (!pos.isNull())
        move(pos);
    resize(settings.value("windowSize", QSize(780, 580)).toSize());
    if (settings.value("windowMaximized", false).toBool())
        setWindowState(windowState() ^ Qt::WindowMaximized);
}

void RxDev::saveWindowState()
{
    QSettings settings;
    settings.setValue("windowState", saveState());
    settings.setValue("windowGeometry", saveGeometry());
    settings.setValue("windowMaximized", isMaximized());
    settings.setValue("windowPos", m_lastPos);
    settings.setValue("windowSize", m_lastSize);
}

/*!\brief creates toolbar
 *
 * Setting up toolbars.
 */
void RxDev::setupToolBar()
{
    QToolBar *toolBar = addToolBar(tr("&Main"));
    toolBar->setObjectName("MainToolBar");
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(ui->actionQuit);
    toolBar->addSeparator();
    toolBar->addAction(ui->actionSettings);


    toolBar = addToolBar(tr("&Launchfile"));
    toolBar->setObjectName("launchToolBar");
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(ui->actionNew_Launchfile);
    toolBar->addAction(ui->actionLoad_Launchfile);
    toolBar->addAction(ui->actionSave_as_Launchfile);

    toolBar = addToolBar(tr("&Mode"));
    toolBar->setObjectName("modeToolBar");
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(ui->actionDrag_Drop);
    toolBar->addAction(ui->actionRemap);
    toolBar->addAction(ui->actionDelete_Item);
    toolBar->addWidget(gview->comboBox_sceneScale);
    connect(gview->comboBox_sceneScale, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(setViewScale(const QString &)));

    toolBar = addToolBar(tr("&run"));
    toolBar->setObjectName("runToolBar");
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(ui->actionStart);
    toolBar->addAction(ui->actionStop);

}

/*!\brief scales connector window
 *
 * rescales the QGraphicsView for the connector tab
 */
void RxDev::setViewScale(const QString &scale)
{
    double newScale = scale.left(scale.indexOf("%")).toDouble() / 100.0;
    QMatrix oldMatrix = gview->matrix();
    gview->resetMatrix();
    gview->translate(oldMatrix.dx(), oldMatrix.dy());
    gview->scale(newScale, newScale);
    ui->statusBar->showMessage(tr("scaling..."), 1000);

}

/*!\brief save dialog
 *
 * Close window dialog which is triggered if you close the main window
 */
bool RxDev::maybeSave()
{
    QMessageBox::StandardButton button = QMessageBox::warning(this, QString(),
                                                              tr("Did you save your progress?\nDo you really want to quit?"),
                                                              QMessageBox::Yes | QMessageBox::Cancel);
    if (button == QMessageBox::Cancel)
        return false;
    if (button == QMessageBox::Yes)
        return true;
    return true;
}

/*!\brief mouse remap mode
 *
 * Sets the mouse to remap mode
 */
void RxDev::on_actionRemap_triggered()
{
    gview->setMode(LaunchFileView::InsertLine);
    gview->setCursor(Qt::ArrowCursor);
    ui->actionRemap->setChecked(true);
    ui->actionDelete_Item->setChecked(false);
    ui->actionDrag_Drop->setChecked(false);
    ui->statusBar->showMessage(tr("remap arrow mode"), 5000);
}

/*!\brief mouse drag&drop mode
 *
 * Sets the mouse to drag and drop mode
 */
void RxDev::on_actionDrag_Drop_triggered()
{
    gview->setMode(LaunchFileView::DragItem);
    gview->setCursor(Qt::PointingHandCursor);
    ui->actionRemap->setChecked(false);
    ui->actionDelete_Item->setChecked(false);
    ui->actionDrag_Drop->setChecked(true);
    ui->statusBar->showMessage(tr("drag&drop mode"), 5000);
}

/*!\brief mouse delete mode
 *
 * Sets the mouse to delete mode
 */
void RxDev::on_actionDelete_Item_triggered()
{
    gview->setMode(LaunchFileView::DeleteItem);
    gview->setCursor(Qt::CrossCursor);
    ui->actionRemap->setChecked(false);
    ui->actionDelete_Item->setChecked(true);
    ui->actionDrag_Drop->setChecked(false);
    ui->statusBar->showMessage(tr("deletion mode"), 5000);
}

/*!\brief load settings
 *
 * Loads settings from the settings file or launches a dialog on first start.
 */
void RxDev::loadSettings(){
    if((settings.value("temrinal").toString()==""))
        settings.setValue("terminal","xterm -hold -e bash -i -c");
    if((settings.value("workingDir").toString()=="")){
        settings.setValue("workingDir",QDir::homePath());

        QMessageBox::information(this, (QString::fromUtf8("first start")),
                                 QString::fromUtf8("<h2>no workingpath specified</h2>"
                                                   "<p>Please select a folder for your ROS-packages in the settings"
                                                   "<p>ROS has to know this path as described in the <a href=http://www.ros.org/wiki/ROS/Tutorials/CreatingPackage>wiki</a>"));
        ui->actionSettings->toggle();
    }

    workingDir = (settings.value("workingDir").toString());
    if((settings.value("currentDir").toString()==""))
        settings.setValue("currentDir",QDir::homePath());
    currentDir.setPath(settings.value("currentDir").toString());
    restoreWindowState();
}

/*!\brief change toolbar
 *
 * Enables and disables toolbar items if the current tab is changed.
 */
void RxDev::changeToolBar(){

    if (ui->tabWidget->currentIndex()==0)   {
        ui->dockWidget_availableNodes->show();
        ui->dockWidget_availablePackages->hide();
        ui->actionNew_Launchfile->setEnabled(true);
        ui->actionLoad_Launchfile->setEnabled(true);
        ui->actionSave_as_Launchfile->setEnabled(true);
        ui->actionDrag_Drop->setEnabled(true);
        ui->actionDelete_Item->setEnabled(true);
        ui->actionRemap->setEnabled(true);
        gview->comboBox_sceneScale->setEnabled(true);
        ui->actionStart->setEnabled(true);
        ui->actionStop->setEnabled(true);
        if (rosLaunch->state()!=2){
            ui->actionStart->setEnabled(true);
            ui->actionStop->setEnabled(false);
        }else{
            ui->actionStart->setEnabled(false);
            ui->actionStop->setEnabled(true);
        }
    } else {
        ui->dockWidget_availableNodes->hide();
        ui->dockWidget_availablePackages->show();
        ui->actionNew_Launchfile->setEnabled(false);
        ui->actionLoad_Launchfile->setEnabled(false);
        ui->actionSave_as_Launchfile->setEnabled(false);
        ui->actionDrag_Drop->setEnabled(false);
        ui->actionDelete_Item->setEnabled(false);
        ui->actionRemap->setEnabled(false);
        gview->comboBox_sceneScale->setEnabled(false);
        ui->actionStart->setEnabled(false);
        ui->actionStop->setEnabled(false);
    }
}


void RxDev::on_actionAbout_rxdev_activated()
{
    QMessageBox::about(this, (QString::fromUtf8("About rxDeveloper")),
                       QString::fromUtf8("<h2>rxdev v.1.2 </h2>"
                                         "<p>Copyright &copy; 2011 Institute for Computer Science VI (AIS)"
                                         "<p>Created by: Filip Müllers (mailto:F.Muellers@gmail.com)"));
}

/*!\brief save launchfile
 *
 * Get target launchfile and collect items for the launchfile.
 */
void RxDev::on_actionSave_as_Launchfile_triggered()
{
    QString file = QFileDialog::getSaveFileName(
                this, tr( "Save launch file or component as ..." ),

                currentDir.absolutePath(), tr("Launchfiles (*.launch *.xml );;Componentfiles (*.comp)"));
    if (!file.isEmpty()){
        settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
        currentDir.setPath(settings.value("currentDir").toString());

        //writing to Launchfile
        LaunchWriter *launchFile = new LaunchWriter;
        QList<QGraphicsItem *> list;
        list=scene->items();
        ui->statusBar->showMessage(tr("writing launchfile..."));
        QString depricated_launch="";
        if (gview->getDeprecated())
            depricated_launch=gview->getMessage();
        launchFile->createDocument(file,list, depricated_launch);
        ui->statusBar->showMessage(tr("Launchfile '%1' has been written").arg(file), 5000);
        setWindowTitle("rxDeveloper - "+file);
        QMessageBox::StandardButton button = QMessageBox::question(this, (QString::fromUtf8("Writing done")),
                                                                   QString::fromUtf8("<h2>Launchfile has been written!</h2>"
                                                                                     "<p>The file "+file.toUtf8()+" was created. Do you want to open this file?</p>"),

                                                                   QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
            QDesktopServices::openUrl(QUrl::fromLocalFile( file));
    }
}


/*!\brief load launchfile
 *
 * Get source launchfile and start the tinyXML.
 */
void RxDev::on_actionLoad_Launchfile_triggered()
{
    QString file;
    file = QFileDialog::getOpenFileName(
                this, tr( "Open Launch File" ),
                currentDir.absolutePath(), tr("Launchfiles (*.launch *.xml)"));
    if (!file.isEmpty()){
        currentDir.setPath(settings.value("currentDir").toString());
        settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
        if( !QFile::exists( file ) )
        {
            QMessageBox::critical( this, tr( "RxDev" ), tr( "The file '%1' does not exist!" ).arg( file ) );
            return;
        }
        std::string utf8_text = file.toUtf8().constData();
        TiXmlDocument doc( utf8_text );
        bool loadOkay = doc.LoadFile();
        if (loadOkay)
        {
            ui->statusBar->showMessage(tr("opening launchfile..."));
            loadDocument( &doc );
            setWindowTitle("rxDeveloper - "+file);
            ui->statusBar->showMessage(tr("Launchfile '%1' has been opened").arg(file), 5000);
        }
        else
        {
            qDebug()<<"\nFailed to load file: "<<file;
        }
    }

}

/*!\brief save settings
 *
 * Saves settings to the settings file.
 */
void RxDev::on_actionSettings_triggered()
{
    Settings settingDialog(workingDir.absolutePath(),settings.value("terminal").toString());
    bool accept = settingDialog.exec();
    if ((accept)){
        workingDir = settingDialog.getWorkingDir();
        settings.setValue("workingDir",settingDialog.getWorkingDir());
        settings.setValue("terminal",settingDialog.getTerminal());
        //update workingDir in the component creator tab
        workingModel->setRootPath(workingDir.absolutePath());
        ui->treeView_packageBrowser->setRootIndex(workingModel->index(workingDir.absolutePath()));
        refresh_packageModel();
        ui->statusBar->showMessage(tr("Settings saved"), 5000);
    } else{

    }
}


void RxDev::on_actionStart_triggered()
{
    QString file=QDir::temp().absolutePath().append("/temp.launch");
    //qDebug()<<file;
    //writing to Launchfile
    LaunchWriter *launchFile = new LaunchWriter;
    QList<QGraphicsItem *> list;
    list=scene->items();
    QString depricated_launch="";
    if (gview->getDeprecated())
        depricated_launch=gview->getMessage();
    launchFile->createDocument(file,list, depricated_launch);
    rosLaunch->setWorkingDirectory(QDir::temp().absolutePath());
    rosLaunch->setProcessChannelMode(QProcess::MergedChannels);
    QString launch= (settings.value("terminal").toString().trimmed()+" \"roslaunch temp.launch\"");
    rosLaunch->start(launch);
    QByteArray output = rosLaunch->readAll();
    //qDebug()<<output;
    QTime time = QTime::currentTime();
    ui->textEdit_Info->append("<font color=\"red\">"+time.toString()+" - Information: <font color=\"blue\">trying to launch computational graph");
    ui->textEdit_Info->append(output.trimmed()+" If nothing happens you should check the settings for the right terminal emulator.");
    ui->dockWidget_errors->show();



}

void RxDev::on_actionStop_triggered()
{
    rosLaunch->kill();
}

void RxDev::on_pushButton_clearInfo_clicked()
{
    ui->textEdit_Info->clear();
}
void RxDev::state(QProcess::ProcessState){
    if (rosLaunch->state()==2){
        ui->actionStop->setEnabled(true);
        ui->actionStart->setEnabled(false);

    }else if (rosLaunch->state()==0){
        ui->actionStart->setEnabled(true);
        ui->actionStop->setEnabled(false);
    }
    if (rxgraph->state()==0){
        ui->actionRxGraph->setChecked(false);
    }
    if (rviz->state()==0){
        ui->actionRviz->setChecked(false);
    }
    if (rxloggerlevel->state()==0){
        ui->actionRxloggerlevel->setChecked(false);
    }
    if (rxconsole->state()==0){
        ui->actionRxconsole->setChecked(false);
    }
    if (dynamicreconfigure->state()==0){
        ui->actionDynamic_reconfigure_gui->setChecked(false);
    }
}


void RxDev::on_actionRxGraph_toggled(bool status)
{
    if (status)
        rxgraph->start(QString("rosrun rxgraph rxgraph"));
    else
        rxgraph->kill();

}

void RxDev::on_actionDynamic_reconfigure_gui_toggled(bool status)
{
    if (status)
        dynamicreconfigure->start(QString("rosrun dynamic_reconfigure reconfigure_gui"));
    else
        dynamicreconfigure->kill();


}

void RxDev::on_actionRxloggerlevel_toggled(bool status)
{

    if (status)
        rxloggerlevel->start(QString("rosrun rxtools rxloggerlevel"));
    else
        rxloggerlevel->kill();

}

void RxDev::on_actionRxconsole_toggled(bool status)
{
    if(status)
        rxconsole->start(QString("rosrun rxtools rxconsole"));
    else
        rxconsole->kill();
}



void RxDev::on_actionRoswtf_triggered()
{
    roswtf->setWorkingDirectory(QDir::temp().absolutePath());
    roswtf->setProcessChannelMode(QProcess::MergedChannels);
    roswtf->start(QString("rosrun roswtf roswtf"));
    roswtf->waitForFinished(-1);
    QByteArray output = roswtf->readAll();
    //qDebug()<<output;
    QTime time = QTime::currentTime();
    ui->textEdit_Info->append("<font color=\"red\">"+time.toString()+" - Information: <font color=\"blue\">roswtf for current launchfile");
    ui->textEdit_Info->append(output.trimmed());
    ui->dockWidget_errors->show();
}

void RxDev::on_actionRviz_toggled(bool status)
{
    if(status)
        rviz->start(QString("rosrun rviz rviz"));
    else
        rviz->kill();
}

void RxDev::on_actionOnline_Help_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.ros.org/wiki/roslaunch/XML"));

}
