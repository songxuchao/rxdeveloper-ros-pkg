
#ifndef RemapEdit_H
#define RemapEdit_H

#include <QDialog>
#include "remapItem.h"


namespace Ui {
    class RemapEdit;
}

class RemapEdit : public QDialog {
        Q_OBJECT

        public:
                explicit RemapEdit(RemapItem *item, QWidget *parent = 0);
                ~RemapEdit();
                QString getFrom();
                QString getTo();
                QString getIf();
                QString getUnless();

        public slots:
                void reject();
                void accept();

        private slots:

        private:
                Ui::RemapEdit *ui;

};

#endif
