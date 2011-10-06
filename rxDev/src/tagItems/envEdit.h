
#ifndef ENVEDIT_H
#define ENVEDIT_H

#include <QDialog>
#include "envItem.h"


namespace Ui {
    class EnvEdit;
}

class EnvEdit : public QDialog {
        Q_OBJECT

        public:
                explicit EnvEdit(EnvItem *item, QWidget *parent = 0);
                ~EnvEdit();
                QString getName();
                QString getValue();
                QString getIf();
                QString getUnless();

        public slots:
                void reject();
                void accept();

        private slots:

        private:
                Ui::EnvEdit *ui;

};

#endif
