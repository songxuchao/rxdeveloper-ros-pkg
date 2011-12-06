
#ifndef INCLUDEFILEEDIT_H
#define INCLUDEFILEEDIT_H

#include <QDialog>
#include "includeFileItem.h"
#include <QStandardItemModel>
#include <QItemSelection>

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
                QString getExpandFile();
                void fillEnvModel();
                void fillArgModel();

        public slots:
                void reject();
                void accept();

        private slots:
            void edit_envItems(const QModelIndex & index);
            void edit_argItems(const QModelIndex & index);
            void selectionHandle_argItems(const QModelIndex & index);
            void selectionHandle_envItems(const QModelIndex & index);
            void on_pushButton_tryOpen_clicked();
            void on_actionDelete_env_triggered();

            void on_actionDelete_arg_triggered();
            void on_checkBox_expand_toggled(bool checked);
            void on_actionAdd_arg_triggered();

            void on_actionAdd_env_triggered();


private:
                Ui::IncludeFileEdit *ui;
                QStandardItemModel *envModel;
                int selectedArg;
                int selectedEnv;
                QStandardItemModel *argModel;
                IncludeFileItem *myItem;
                QString expandFile;

};

#endif
