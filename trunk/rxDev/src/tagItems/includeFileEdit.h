
#ifndef INCLUDEFILEEDIT_H
#define INCLUDEFILEEDIT_H

#include <QDialog>
#include "includeFileItem.h"
#include <QStandardItemModel>


namespace Ui {
    class IncludeFileEdit;
}

class IncludeFileEdit : public QDialog {
        Q_OBJECT

        public:
                explicit IncludeFileEdit(IncludeFileItem *item, QWidget *parent = 0);
                ~IncludeFileEdit();
                QString getNamespace();
                QString getFile();
                int getClearParam();
                QString getIf();
                QString getUnless();

        public slots:
                void reject();
                void accept();

        private slots:

        private:
                Ui::IncludeFileEdit *ui;
                QStandardItemModel *envModel;

                QStandardItemModel *argModel;

};

#endif
