
#ifndef NODEEDIT_H
#define NODEEDIT_H

#include <QDialog>
#include "nodeItem.h"
#include <QStandardItemModel>



namespace Ui {
class NodeEdit;
}

class NodeEdit : public QDialog {
    Q_OBJECT

public:
    explicit NodeEdit(NodeItem *item,bool blank, QWidget *parent = 0);
    ~NodeEdit();
    QString getName();
    QString getType();
    QString getPackage();
    QString getArgs();
    QString getNamespace();
    QString getMachine();
    QString getTime_limit();
    QString getRetry();
    QString getLaunch_prefix();
    QString getIf();
    QString getUnless();
    void fillEnvModel();
    void fillRemapModel();
    void fillParameterModel();
    void fillRosparamModel();
    int getRequired();
    int getRespawn();
    int getClear_params();
    int getOutput();
    int getCwd();
    int node_or_test;

public slots:
    void reject();
    void accept();

private slots:
    void edit_parameterItems(const QModelIndex & index);
    void edit_rosparamItems(const QModelIndex & index);
    void edit_envItems(const QModelIndex & index);
    void edit_remapItems(const QModelIndex & index);
    void selectionHandle_envItems(const QModelIndex & index);
    void selectionHandle_remapItems(const QModelIndex & index);
    void selectionHandle_parameterItems(const QModelIndex & index);
    void selectionHandle_rosparamItems(const QModelIndex & index);

private:
    Ui::NodeEdit *ui;
    QStandardItemModel *envModel;
    QStandardItemModel *paramModel;
    QStandardItemModel *rosparamModel;
    QStandardItemModel *remapModel;
    NodeItem *myItem;
    int selectedEnv;
    int selectedRemap;
    int selectedParameter;
    int selectedRosparam;


signals:

private slots:
    void on_radioButton_testNode_clicked();
    void on_radioButton_standardNode_clicked();
    void on_actionDelete_env_triggered();
    void on_actionDelete_rosparam_triggered();
    void on_actionDelete_param_triggered();
    void on_actionDelete_remap_triggered();
    void on_actionAdd_param_triggered();
    void on_actionAdd_env_triggered();
    void on_actionAdd_remap_triggered();
    void on_actionAdd_rosparam_triggered();
};

#endif
