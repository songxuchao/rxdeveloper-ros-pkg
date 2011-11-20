
#ifndef RemapEdit_H
#define RemapEdit_H

#include <QDialog>
#include "remapArrow.h"
#include <QStandardItemModel>
#include <QItemSelection>


namespace Ui {
    class RemapArrowEdit;
}

class RemapArrowEdit : public QDialog {
        Q_OBJECT

        public:
                explicit RemapArrowEdit(RemapArrow *item, QWidget *parent = 0);
                ~RemapArrowEdit();
                QString getFrom();
                QString getTo();
                QString getIf();
                QString getUnless();
                void updateFrom();
                void updateTo();
                bool getSubToPub();

        public slots:
                void reject();
                void accept();

        private slots:
                void selectionHandle_start_subscribe(const QItemSelection &selected, const QItemSelection &deselected);
                void selectionHandle_end_publish(const QItemSelection &selected, const QItemSelection &deselected);
                void selectionHandle_start_publish(const QItemSelection &selected, const QItemSelection &deselected);
                void selectionHandle_end_subscribe(const QItemSelection &selected, const QItemSelection &deselected);

                void on_checkBox_startName_toggled(bool checked);

                void on_checkBox_targetName_toggled(bool checked);

                void on_checkBox_startNamespace_toggled(bool checked);

                void on_checkBox_targetNamespace_toggled(bool checked);

                void on_checkBox_startGroupNs_toggled(bool checked);

                void on_checkBox_targetGroupNs_toggled(bool checked);

private:
                bool subToPub;
                Ui::RemapArrowEdit *ui;
                QStandardItemModel *model_start_subscribe;
                QStandardItemModel *model_end_publish;
                QStandardItemModel *model_start_publish;
                QStandardItemModel *model_end_subscribe;
                QString fromNode;
                QString fromNodeNamespace;
                QString fromGroupNamespace;
                QString fromString;
                QString toString;
                QString toNode;
                QString toNodeNamespace;
                QString toGroupNamespace;
                RemapArrow *myItem;
//                QString _fromSubscription;
//                QString _toPublication;

};

#endif
