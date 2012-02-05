
#ifndef LAUNCHEDIT_H
#define LAUNCHEDIT_H

#include <QDialog>
#include "../launchFileView.h"



namespace Ui {
    class LaunchEdit;
}

class LaunchEdit : public QDialog {
        Q_OBJECT

        public:
                explicit LaunchEdit(LaunchFileView *item, QWidget *parent = 0);
                ~LaunchEdit();
                QString getMessage();
                bool getDeprecated();


        public slots:
                void reject();
                void accept();

        private slots:

private:
                Ui::LaunchEdit *ui;

};

#endif
