#include "rxdev.h"
#include "ui_rxdev.h"

#include <QCloseEvent>
#include <QComboBox>
#include <QToolBar>
#include <QUrl>
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
    ui->tabWidget->setCurrentIndex(0);  //0 = connector tab

    //load Settings
    loadSettings();

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


    toolBar = addToolBar(tr("&Launchfile"));
    toolBar->setObjectName("launchToolBar");
    toolBar->setIconSize(QSize(16, 16));
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

        ui->actionLoad_Launchfile->setEnabled(true);
        ui->actionSave_as_Launchfile->setEnabled(true);
        ui->actionLoad_Launchfile->setEnabled(true);
        ui->actionSave_as_Launchfile->setEnabled(true);
        ui->actionDrag_Drop->setEnabled(true);
        ui->actionDelete_Item->setEnabled(true);
        ui->actionRemap->setEnabled(true);
    } else {

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


/*!\brief save launchfile
 *
 * Get target launchfile and collect items for the launchfile.
 */
void RxDev::on_actionSave_as_Launchfile_triggered()
{
    QString file = QFileDialog::getSaveFileName(
                this, tr( "Save launch file as ..." ),

                currentDir.absolutePath(), tr("Launchfiles (*.launch *.xml)"));
    if (!file.isEmpty()){
        settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
        currentDir.setPath(settings.value("currentDir").toString());

        //writing to Launchfile
        LaunchWriter *launchFile = new LaunchWriter;
        QList<QGraphicsItem *> list;
        list=scene->items();
        ui->statusBar->showMessage(tr("writing launchfile..."));
        launchFile->createDocument(file,list);
        ui->statusBar->showMessage(tr("Launchfile '%1' has been written").arg(file), 5000);
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
            ui->statusBar->showMessage(tr("opening launchfile..."));
            loadDocument( &doc );
            ui->statusBar->showMessage(tr("Launchfile '%1' has been opened").arg(file), 5000);
        }
        else
        {
            qDebug()<<"\nFailed to load file "<<file;
        }
    }

}

/*!\brief save settings
 *
 * Saves settings to the settings file.
 */
void RxDev::on_actionSettings_triggered()
{
    Settings settingDialog(workingDir.absolutePath());
    bool accept = settingDialog.exec();
    if ((accept)){
        workingDir = settingDialog.getWorkingDir();
        settings.setValue("workingDir",settingDialog.getWorkingDir());

        //update workingDir in the component creator tab
        workingModel->setRootPath(workingDir.absolutePath());
        ui->treeView_packageBrowser->setRootIndex(workingModel->index(workingDir.absolutePath()));
        ui->statusBar->showMessage(tr("Settings saved"), 5000);
    } else{

    }
}
