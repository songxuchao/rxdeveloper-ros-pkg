#ifndef RXDEV_H
#define RXDEV_H


#include "ros/ros.h"
#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>

//#include <QFileSystemWatcher>
#include <QStringListModel>
#include <QStringList>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QThread>

#include "QDebug"
#include "QFile"
#include "QDir"
#include "QDesktopServices"
#include "QTimer"

#include "QRegExp"
#include <QSettings>

#include <QStandardItemModel>

#include "launchFileView.h"
#include "launchFileScene.h"

#include <tinyxml.h>
#include "tagItems/argItem.h"
#include "tagItems/remapItem.h"
#include "tagItems/remapArrow.h"
#include "tagItems/includeFileItem.h"
#include "tagItems/machineItem.h"
#include "tagItems/groupItem.h"
#include "specFileParser.h"

class LaunchFileScene;
class LaunchFileView;





namespace Ui {
    class RxDev;
    class UpdateNodesThread;

}

class RxDev : public QMainWindow
{
    Q_OBJECT

public:
    explicit RxDev(QWidget *parent = 0);
    QSettings settings;

    ~RxDev();


public slots:
    void loadDocument( TiXmlNode * pParent);
    void loadDocumentInGroup( TiXmlNode * documentNode, GroupItem &group);

private slots:
    void setupCreator();
    void selectionHandle_packages(const QItemSelection &,const QItemSelection &);
    void selectionHandle_selectedPackage(const QItemSelection &,const QItemSelection &);
    void selectionHandle_availableNodes(const QItemSelection &,const QItemSelection &);
    void on_pushButton_createPackage_clicked();
    void on_actionAbout_rxdev_activated();
    void changeToolBar();

    void enableCreatePackageButton(const QString &text);

    void openFileOrFolder();
    void showContextMenu(const QPoint &point);

    void deleteFile();
    void createNewFile();
    void createNewFolder();

    void renameFolder();

    void loadSettings();


    //new

    void availableNodesOrComps();
    void showContextMenu_availableNodes(const QPoint &point);
    void showContextMenu_availableComponents(const QPoint &point);
    void openCompFile();
    void addCompFile();

    void openSpecFile();

    void collapseNode();
    void expandNode();
    void collapseAll();
    void expandAll();


    void fillItemModel_availableNodes(const QString nodeFile, Specfile &node);
    void fillItemModel_availableComponents(const QString compFile);

    void setupConnector();
    //end new
    void on_actionLoad_Launchfile_triggered();

    void on_actionSave_as_Launchfile_triggered();



    void on_actionRemap_triggered();

    void on_actionDrag_Drop_triggered();

    void on_actionDelete_Item_triggered();

// new 19.08

    bool maybeSave();
    void setViewScale(const QString &scale);

    //end 19.08

    //new 21.08

    //void itemInserted(NodeItem *item);

    //end 21.08
    void on_pushButton_Param_clicked();
    //5.09.
    QPointF findSpace(QPointF currentPoint);

    void on_pushButton_includeFile_clicked();

    void on_pushButton_machine_clicked();

    void on_pushButton_arg_clicked();

    void on_pushButton_env_clicked();

    void on_pushButton_group_clicked();




    void expandInclude(const QString &string,GroupItem &group);
    void create_groupItem(TiXmlNode *pParent,GroupItem* newGroup);
    void create_paramItem(ParameterItem &newParam, TiXmlNode * pParent,int &x, int &y);
    void create_rosparamItem(RosparamItem &newRosparam, TiXmlNode * pParent,int &x, int &y);
    void create_envItem(EnvItem &newEnv,TiXmlNode * pParent,int &x, int &y);
    void create_argItem(ArgItem &newArg,TiXmlNode * pParent,int &x, int &y);
    void create_remapItem(RemapItem &newRemap, TiXmlNode * pParent,int &x, int &y);
    void create_remapArrow(const remapArrowData *arrowData);
    void create_includeFileItem(IncludeFileItem &newInclude,TiXmlNode *pParent,int &x, int &y);
    void create_machineItem(MachineItem &nemMachine, TiXmlNode *pParent,int &x, int &y);
    void prepare_nodeOrTest(TiXmlNode *nodeNode,QString &nodePackage, QString &nodeType,QStringList &nodeSubs,
                            QStringList &nodePubs,QStringList &nodeArgs,QStringList &nodeSrvs);
    void create_nodeorTestItem(NodeItem &newNode, int nodeOrTest,TiXmlNode * pParent,int &x, int &y);
    void on_pushButton_remap_clicked();
    void beginParsing(TiXmlNode *pParent);
    QPoint getCoords(TiXmlNode *node);
    void getGroupDim(TiXmlNode *node,int &width, int &height);
    void on_pushButton_refreshNodesOrComps_clicked();


    void on_actionSettings_triggered();

    void on_actionNew_Launchfile_triggered();

    void on_pushButton_rosparam_clicked();

    void on_actionStart_triggered();

    void on_actionStop_triggered();

    void on_pushButton_clearInfo_clicked();
    void state(QProcess::ProcessState);

    void on_pushButton_blankNodelet_clicked();

    void on_pushButton_managerNodelet_clicked();

    void on_pushButton_blankNode_clicked();

    void on_actionRxGraph_toggled(bool status);

    void on_actionDynamic_reconfigure_gui_toggled(bool status);

    void on_actionRxloggerlevel_toggled(bool status);

    void on_actionRxconsole_toggled(bool status);

    void on_actionRviz_toggled(bool status);

    void on_pushButton_createCpp_clicked();

    void on_pushButton_createPython_clicked();

    void on_pushButton_specfile_clicked();
    void on_pushButton_specfilFromExe_clicked();

    void refresh_packageModel();

    void on_actionRoswtf_triggered();


    void on_actionOnline_Help_triggered();


private:
    Ui::RxDev *ui;
    QLabel *rosCoreStatus;
    QPushButton *rosCoreButton;
    QProcess *rosCoreProcess;
    QProcess *roswtf;
    QProcess *rosnode;
    QProcess *rviz;
    QProcess *rosLaunch;
    QProcess *rxgraph;
    QProcess *rxloggerlevel;
    QProcess *rxconsole;
    QProcess *dynamicreconfigure;
    QStringListModel *model;
    QStringListModel *model_2;
    QFileSystemModel *workingModel;
    QStringListModel *packageModel;
    QStandardItemModel *model_availableNodes;
    QStandardItemModel *model_availableComponents;
    QString folderPath;
    QString packagePath;
    //QFileSystemWatcher *watcher;
    QStringList workingDirPackages;
    QStringList packageList;
    QStringList nodeList;

    bool check;
    bool project;
    QDir workingDir;
    QDir currentDir;

    int tiny_version;
    //new
     //end new

    //new 19.08
    QPoint m_lastPos;
    QSize m_lastSize;

LaunchFileScene * scene;
LaunchFileView * gview; ///< the graphics view will contain and display the scene.



protected:

    void setupToolBar();
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    //void showEvent(QShowEvent *event);

    void saveWindowState();
    void restoreWindowState();
    //end 19.08
};


#endif // RXDEV_H






