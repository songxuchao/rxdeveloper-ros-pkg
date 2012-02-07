
#ifndef ROSPARAMEDIT_H
#define ROSPARAMEDIT_H

#include <QDialog>
#include "rosparamItem.h"



namespace Ui {
    class RosparamEdit;
}

class RosparamEdit : public QDialog {
        Q_OBJECT

        public:
                explicit RosparamEdit(RosparamItem *item, QWidget *parent = 0);
                ~RosparamEdit();
                QString getName();
                QString getValue();
                QString getType();
                QString getNamespace();
                QString getText();
                QString getIf();
                QString getUnless();

        public slots:
                void reject();
                void accept();

        private slots:
                void on_pushButton_tryOpen_clicked();

        private:
                Ui::RosparamEdit *ui;

private slots:

};

#endif
