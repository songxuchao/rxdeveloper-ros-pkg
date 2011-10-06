
#ifndef ARGEDIT_H
#define ARGEDIT_H

#include <QDialog>
#include "argItem.h"



namespace Ui {
    class ArgEdit;
}

class ArgEdit : public QDialog {
        Q_OBJECT

        public:
                explicit ArgEdit(ArgItem *item, QWidget *parent = 0);
                ~ArgEdit();
                QString getName();
                QString getValue();
                QString getIf();
                QString getUnless();
                int getValue_or_default();


        public slots:
                void reject();
                void accept();

        private slots:

                void on_comboBox_currentIndexChanged(int index);

private:
                Ui::ArgEdit *ui;

};

#endif
