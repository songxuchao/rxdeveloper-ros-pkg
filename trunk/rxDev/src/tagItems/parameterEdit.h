
#ifndef PARAMETEREDIT_H
#define PARAMETEREDIT_H

#include <QDialog>
#include "parameterItem.h"



namespace Ui {
    class ParameterEdit;
}

class ParameterEdit : public QDialog {
        Q_OBJECT

        public:
                explicit ParameterEdit(ParameterItem *item, QWidget *parent = 0);
                ~ParameterEdit();
                QString getName();
                QString getValue();
                QString getType();
                QString getNamespace();
                QString getIf();
                QString getUnless();
                int getParamType();

        public slots:
                void reject();
                void accept();

        private slots:

        private:
                Ui::ParameterEdit *ui;
                int param_type;
private slots:

                void on_radioButton_file_clicked();
                void on_radioButton_standard_clicked();
                void on_radioButton_rosparam_clicked();
};

#endif
