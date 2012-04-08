
#ifndef NODESELECTION_H
#define NODESELECTION_H

#include <QDialog>
#include <QStringListModel>
#include "rxdev.h"

namespace Ui {
    class NodeSelection;
}

class NodeSelection : public QDialog {
        Q_OBJECT

        public:
                explicit NodeSelection(QStringList list, QWidget *parent = 0);
                ~NodeSelection();
                int getNode();

        public slots:
                void reject();
                void accept();

        private slots:
                void selectionHandle_nodeItem(const QModelIndex & index);
                void selectionHandle_nodeItem_accepted(const QModelIndex & index);


        private:
                Ui::NodeSelection *ui;
                QStringListModel *nodeModel;
                int selectedNode;

private slots:

};

#endif
