#include "rxdev.h"
#include "ui_rxdev.h"

#include <QCloseEvent>
#include <QComboBox>
#include <QToolBar>
#include "launchFileWriter.h"
#include "launchFileReader.h"


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
    ui->tabWidget->setCurrentIndex(0);  //0 = connector tab

    //load Settings
    ui->actionSave_settings->setEnabled(false);
    loadSettings();
    ui->tabWidget->removeTab(2);    //remove the settings tab

    //initialize connector tab
    setupConnector();
    //initialize list of available nodes and adding data
    availableNodes();

    //create Toolbar
    setupToolBar();

    ui->statusBar->showMessage(tr("Ready"));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(changeToolBar()));

    //initialize the creator tab
    setupCreator();

    setUnifiedTitleAndToolBarOnMac(true);
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
    toolBar->addAction(ui->actionSave_settings);

    toolBar = addToolBar(tr("&Launchfile"));
    toolBar->setObjectName("launchToolBar");
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(ui->actionLoad_Project);
    toolBar->addAction(ui->actionSave_Project);
    toolBar->addAction(ui->actionLoad_Launchfile);
    toolBar->addAction(ui->actionSave_as_Launchfile);

    toolBar = addToolBar(tr("&Mode"));
    toolBar->setObjectName("modeToolBar");
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(ui->actionDrag_Drop);
    toolBar->addAction(ui->actionRemap);
    toolBar->addAction(ui->actionDelete_Item);

    toolBar = addToolBar(tr("&View"));
    toolBar->setObjectName("viewToolBar");
    toolBar->addWidget(gview->comboBox_sceneScale);
    connect(gview->comboBox_sceneScale, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(setViewScale(const QString &)));

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
}

/*!\brief load settings
 *
 * Loads settings from the settings file or launches a dialog on first start.
 */
void RxDev::loadSettings(){
    if((settings.value("workingDir").toString()=="")){
        settings.setValue("workingDir",QDir::homePath());

        QMessageBox::information(this, (QString::fromUtf8("first start")),
                                 QString::fromUtf8("<h2>no workingpath specified</h2>"
                                                   "<p>Please select a folder for your ROS-packages in the settings"
                                                   "<p>ROS has to know this path as described in the <a href=http://www.ros.org/wiki/ROS/Tutorials/CreatingPackage>wiki</a>"));
        ui->actionSettings->toggle();
    }

    ui->lineEdit_workingDir->setText(settings.value("workingDir").toString());
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
        ui->actionLoad_Project->setEnabled(true);
        ui->actionSave_Project->setEnabled(true);
        ui->actionLoad_Launchfile->setEnabled(true);
        ui->actionSave_as_Launchfile->setEnabled(true);
        ui->actionLoad_Launchfile->setEnabled(true);
        ui->actionSave_as_Launchfile->setEnabled(true);
        ui->actionDrag_Drop->setEnabled(true);
        ui->actionDelete_Item->setEnabled(true);
        ui->actionRemap->setEnabled(true);
    } else {
        ui->actionLoad_Project->setEnabled(false);
        ui->actionSave_Project->setEnabled(false);
        ui->actionLoad_Launchfile->setEnabled(false);
        ui->actionSave_as_Launchfile->setEnabled(false);
        ui->actionLoad_Launchfile->setEnabled(false);
        ui->actionSave_as_Launchfile->setEnabled(false);
        ui->actionDrag_Drop->setEnabled(false);
        ui->actionDelete_Item->setEnabled(false);
        ui->actionRemap->setEnabled(false);
    }
}


void RxDev::on_actionAbout_rxdev_activated()
{
    QMessageBox::about(this, (QString::fromUtf8("About rxDeveloper")),
                       QString::fromUtf8("<h2>rxdev v.0.5</h2>"
                                         "<p>Copyright &copy; 2011 Institute for Computer Science VI (AIS)"
                                         "<p>Created by: Filip Müllers (mailto:mueller4@cs.uni-bonn.de)"));
}


/*!\brief show available nodes?
 *
 * Shows and hides the available nodes dock.
 */
void RxDev::on_actionAvailable_Nodes_changed()
{
    if (ui->actionAvailable_Nodes->isChecked()==true)
        ui->dockWidget_availableNodes->show();
    else
        ui->dockWidget_availableNodes->hide();
}

/*!\brief show available tags?
 *
 * Shows and hides the available tags dock.
 */
void RxDev::on_actionAvailable_Tags_changed()
{
    if (ui->actionAvailable_Tags->isChecked()==true)
        ui->dockWidget_availableTags->show();
    else
        ui->dockWidget_availableTags->hide();

}

/*!\brief save settings
 *
 * Saves settings to the settings file.
 */
void RxDev::on_actionSave_settings_triggered()
{
    workingDir = ui->lineEdit_workingDir->text();
    settings.setValue("workingDir",ui->lineEdit_workingDir->text());
    QMessageBox::information(this, (QString::fromUtf8("Information")),
                             QString::fromUtf8("<h2>Settings saved</h2>"
                                               "<p>Press 'OK' to continue!</p>"));
    workingModel->setRootPath(workingDir.absolutePath());
    ui->treeView_packageBrowser->setRootIndex(workingModel->index(workingDir.absolutePath()));

}

/*!\brief working direcory dialog
 *
 * Launches the working directory dialog.
 */
void RxDev::on_pushButton_browseWorkingDir_clicked()
{
    QString sourceFolder = QFileDialog::getExistingDirectory(this, tr("Choose working directory"), QDir::homePath());
    if (!sourceFolder.isEmpty())
        ui->lineEdit_workingDir->setText( sourceFolder);
}

/*!\brief open/close settings
 *
 * Enables and disables toolbar items if the current tab is changed.
 */
void RxDev::on_actionSettings_toggled(bool )
{
    if (ui->actionSettings->isChecked()==true){
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);
        ui->tabWidget->insertTab(2, ui->tab_5, "Settings");
        ui->actionLoad_Project->setEnabled(false);
        ui->actionSave_Project->setEnabled(false);
        ui->actionLoad_Launchfile->setEnabled(false);
        ui->actionSave_as_Launchfile->setEnabled(false);
        ui->actionSave_settings->setEnabled(true);
        ui->actionDrag_Drop->setEnabled(false);
        ui->actionDelete_Item->setEnabled(false);
        ui->actionRemap->setEnabled(false);
    } else{
        ui->tabWidget->removeTab(0);
        ui->tabWidget->insertTab(0, ui->tab, "Component Connector");
        ui->tabWidget->insertTab(1, ui->tab_2, "Component Creator");
        ui->actionSave_settings->setEnabled(false);
    }
}

/*!\brief save launchfile
 *
 * Get target launchfile and collect items for the launchfile.
 */
void RxDev::on_actionSave_as_Launchfile_triggered()
{
    QString file = QFileDialog::getSaveFileName(
                this, tr( "Save launch file as ..." ),

                currentDir.absolutePath(), tr("Launchfiles (*.launch *.xml)"));

    settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
    currentDir.setPath(settings.value("currentDir").toString());

    //writing to Launchfile
    LaunchWriter *launchFile = new LaunchWriter;
    QList<QGraphicsItem *> list;
    list=scene->items();
    launchFile->createDocument(file,false,list);    //false = create launchfile

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
    currentDir.setPath(settings.value("currentDir").toString());
    settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
    if( !QFile::exists( file ) )
    {
        QMessageBox::critical( this, tr( "RxDev" ), tr( "The file \"%1\" does not exist!" ).arg( file ) );
        return;
    }

    /*    QFile launchFile(file);
    //loading and parsing launchfile
    launchFile.open(QIODevice::ReadOnly);
    while (!launchFile.atEnd())
    {
        QString line = launchFile.readLine();

        if(!line.trimmed().isEmpty()){
            qDebug()<<line;
        }
    }
    launchFile.close();
*/
    TiXmlDocument doc( file.toStdString() );
    bool loadOkay = doc.LoadFile();
    if (loadOkay)
    {
        qDebug()<<"\n:"<<file;
        loadDocument( &doc );
    }
    else
    {
        qDebug()<<"\nFailed to load file "<<file;
    }


}


/*!\brief load project file
 *
 * Get source project file and start the tinyXML with project set true.
 */
void RxDev::on_actionLoad_Project_triggered()
{
    project=true;
    QString file;
    file = QFileDialog::getOpenFileName(
                this, tr( "Open Project File" ),
                currentDir.absolutePath(), tr("*.rxd (*.rxd)"));
    currentDir.setPath(settings.value("currentDir").toString());
    settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
    if( !QFile::exists( file ) )
    {
        QMessageBox::critical( this, tr( "RxDev" ), tr( "The file \"%1\" does not exist!" ).arg( file ) );
        return;
    }
    TiXmlDocument doc( file.toStdString() );
    bool loadOkay = doc.LoadFile();
    if (loadOkay)
    {
        qDebug()<<"\n:"<<file;
        loadDocument( &doc );
    }
    else
    {
        qDebug()<<"\nFailed to load file "<<file;
    }

    project=false;


}

/*!\brief save project file
 *
 * Get target project file, collect items for the project file and make it produce project information
 */
void RxDev::on_actionSave_Project_triggered()
{

    QString file = QFileDialog::getSaveFileName(
                this, tr( "Save project as ..." ),

                currentDir.absolutePath(), tr("*.rxd (*.rxd)"));

    settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
    currentDir.setPath(settings.value("currentDir").toString());

    //writing to file
    LaunchWriter *launchFile = new LaunchWriter;
    QList<QGraphicsItem *> list;
    list=scene->items();
    launchFile->createDocument(file,true,list); //true = create projectfile
}
