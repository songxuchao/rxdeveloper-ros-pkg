
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
    explicit NodeEdit(NodeItem *item, QWidget *parent = 0);
    ~NodeEdit();
    QString getName();
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
    void selectionHandle_parameterItems(const QModelIndex & index);
    void selectionHandle_rosparamItems(const QModelIndex & index);
    void selectionHandle_envItems(const QModelIndex & index);
    void selectionHandle_remapItems(const QModelIndex & index);
private:
    Ui::NodeEdit *ui;
    QStandardItemModel *envModel;
    QStandardItemModel *paramModel;
    QStandardItemModel *rosparamModel;
    QStandardItemModel *remapModel;
    NodeItem *myItem;
    QList<int> paramItemsList;          //List of parameterItems
    QList<int> rosparamItemsList;          //List of rosparamItems

signals:

private slots:
    void on_radioButton_testNode_clicked();
    void on_radioButton_standardNode_clicked();
};

#endif
