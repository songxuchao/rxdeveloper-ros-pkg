
#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QDialog>
#include "rxdev.h"


namespace Ui {
class WikiDialog;
}

class WikiDialog : public QDialog {
    Q_OBJECT

public:
    explicit WikiDialog(QWidget *parent = 0);
    ~WikiDialog();
    void setText(QString text);

public slots:
    void reject();
    void accept();

private slots:

private:
    Ui::WikiDialog *ui;

private slots:

    void on_pushButton_clipboard_clicked();
};

#endif
