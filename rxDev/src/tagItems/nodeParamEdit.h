
#ifndef NODEPARAMEDIT_H
#define NODEPARAMEDIT_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
    class NodeParamEdit;
}

class NodeParamEdit : public QDialog {
        Q_OBJECT

        public:
                explicit NodeParamEdit(QStringList list, QWidget *parent = 0);
                ~NodeParamEdit();
                int getParam();

        public slots:
                void reject();
                void accept();

        private slots:
                void selectionHandle_paramItem(const QModelIndex & index);
                void selectionHandle_paramItem_accepted(const QModelIndex & index);


        private:
                Ui::NodeParamEdit *ui;
                QStandardItemModel *paramModel;
                int selectedParam;

private slots:

};

#endif
