
#ifndef SPECFILEEDIT_H
#define SPECFILEEDIT_H

#include <QDialog>
#include "rxdev.h"
#include "specFileParser.h"


namespace Ui {
class SpecFileEdit;
}

class SpecFileEdit : public QDialog {
    Q_OBJECT

public:
    explicit SpecFileEdit(rosNode *node, QWidget *parent = 0);
    ~SpecFileEdit();

public slots:
    void reject();
    void accept();
    void selectionHandle_subs(const QModelIndex &index);
    void selectionHandle_pubs(const QModelIndex &index);
    void selectionHandle_servs(const QModelIndex &index);
    void selectionHandle_params(const QModelIndex &index);
    void on_actionDelete_sub_triggered();
    void on_actionAdd_sub_triggered();
    void on_actionDelete_pub_triggered();
    void on_actionAdd_pub_triggered();
    void on_actionDelete_serv_triggered();
    void on_actionAdd_serv_triggered();
    void on_actionDelete_param_triggered();
    void on_actionAdd_param_triggered();

private slots:

private:
    Ui::SpecFileEdit *ui;
    QStringListModel *model_subscriptions;
    QStringListModel *model_publications;
    QStringListModel *model_services;
    QStringListModel *model_parameters;
    rosNode *mynode;
    int selectedSub,selectedPub,selectedServ,selectedParam;
private slots:

};

#endif
