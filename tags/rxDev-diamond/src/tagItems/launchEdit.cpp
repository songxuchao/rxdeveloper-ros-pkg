#include "ui_launchEdit.h"
#include "launchEdit.h"
#include <QMessageBox>


LaunchEdit::LaunchEdit(LaunchFileView *item, QWidget *parent) : QDialog(parent), ui(new Ui::LaunchEdit) {
    ui->setupUi(this);
    ui->checkBox_deprecated->setChecked(item-> getDeprecated());
    ui->lineEdit_message->setText(item->getMessage());
    ui->lineEdit_message->setFocus();
}

LaunchEdit::~LaunchEdit() {
    delete ui;
}

void LaunchEdit::reject() {

    QDialog::reject();
}

void LaunchEdit::accept() {

    if (ui->checkBox_deprecated->isChecked()==true && ui->lineEdit_message->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No message specified</h2>"
                                                   "<p>Please insert the deprecation message!</p>"));
        ui->lineEdit_message->setFocus();

    } else{

        QDialog::accept();
    }

}

QString LaunchEdit::getMessage(){
    return ui->lineEdit_message->text();
}


bool LaunchEdit::getDeprecated()
{
    return ui->checkBox_deprecated->isChecked();
}
