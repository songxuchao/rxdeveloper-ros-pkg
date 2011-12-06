
#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QDialog>
#include "rxdev.h"


namespace Ui {
class NewEntry;
}

class NewEntry : public QDialog {
    Q_OBJECT

public:
    explicit NewEntry(QWidget *parent = 0);
    ~NewEntry();
    QString getFileName();

public slots:
    void reject();
    void accept();

private slots:

private:
    Ui::NewEntry *ui;

private slots:

};

#endif
