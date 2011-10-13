
#ifndef MACHINEEDIT_H
#define MACHINEEDIT_H

#include <QDialog>
#include "machineItem.h"
#include <QStandardItemModel>



namespace Ui {
class MachineEdit;
}

class MachineEdit : public QDialog {
    Q_OBJECT

public:
    explicit MachineEdit(MachineItem *item, QWidget *parent = 0);
    ~MachineEdit();
    QString getName();
    QString getAddress();
    QString getRos_root();
    QString getRos_package_path();
    QString getDefault();
    QString getUser();
    QString getPassword();
    QString getTimeout();
    QString getIf();
    QString getUnless();
    void fillEnvModel();

public slots:
    void reject();
    void accept();

private slots:
    void selectionHandle_envItems(const QModelIndex & index);


private:
    Ui::MachineEdit *ui;
    QStandardItemModel *envModel;
    MachineItem *myItem;

};

#endif
