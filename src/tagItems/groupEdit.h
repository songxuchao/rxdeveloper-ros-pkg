
#ifndef GROUPEDIT_H
#define GROUPEDIT_H

#include <QDialog>
#include "groupItem.h"



namespace Ui {
    class GroupEdit;
}

class GroupEdit : public QDialog {
        Q_OBJECT

        public:
                explicit GroupEdit(GroupItem *item, QWidget *parent = 0);
                ~GroupEdit();
                QString getNamespace();
                int getClear_params();
                int getWidth();
                int getHeight();
                QString getIf();
                QString getUnless();

        public slots:
                void reject();
                void accept();

        private slots:

        private:
                Ui::GroupEdit *ui;

};

#endif
