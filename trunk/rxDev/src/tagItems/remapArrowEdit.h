
#ifndef RemapEdit_H
#define RemapEdit_H

#include <QDialog>
#include "remapArrow.h"
#include <QStringListModel>


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
                bool getSubToPub();
        public slots:
                void reject();
                void accept();

        private slots:
                void selectionHandle_start_subscribe(const QItemSelection &selected, const QItemSelection &deselected);
                void selectionHandle_end_publish(const QItemSelection &selected, const QItemSelection &deselected);
                void selectionHandle_start_publish(const QItemSelection &selected, const QItemSelection &deselected);
                void selectionHandle_end_subscribe(const QItemSelection &selected, const QItemSelection &deselected);

        private:
                bool subToPub;
                Ui::RemapArrowEdit *ui;
                QStringListModel *model_start_subscribe;
                QStringListModel *model_end_publish;
                QStringListModel *model_start_publish;
                QStringListModel *model_end_subscribe;
//                QString _fromSubscription;
//                QString _toPublication;

};

#endif
