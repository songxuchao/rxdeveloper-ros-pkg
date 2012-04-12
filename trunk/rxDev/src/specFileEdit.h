
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
    explicit SpecFileEdit(Specfile *node, QWidget *parent = 0);
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
    QStandardItemModel *model_subscriptions;
    QStandardItemModel *model_publications;
    QStandardItemModel *model_services;
    QStandardItemModel *model_parameters;
    Specfile *mynode;
    int selectedSub,selectedPub,selectedServ,selectedParam;
private slots:

    void on_pushButton_wiki_clicked();
    void on_pushButton_createCpp_clicked();
};

#endif
