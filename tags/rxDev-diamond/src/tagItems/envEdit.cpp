#include "ui_envEdit.h"
#include "envEdit.h"
#include <QMessageBox>

EnvEdit::EnvEdit(EnvItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::EnvEdit) {
    ui->setupUi(this);
    ui->lineEdit_name->setText(item->getName());
    ui->lineEdit_value->setText(item->getValue());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());

}

EnvEdit::~EnvEdit() {
    delete ui;
}

void EnvEdit::reject() {
    ui->lineEdit_name->setText("");
    QDialog::reject();
}

void EnvEdit::accept() {

    if ( ui->lineEdit_name->text()=="" || ui->lineEdit_value->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No name or value specified</h2>"
                                                   "<p>Please insert the correct name and value!</p>"));
        ui->lineEdit_name->setFocus();

    } else{

        QDialog::accept();
    }

}

QString EnvEdit::getName(){
    return ui->lineEdit_name->text();
}
QString EnvEdit::getValue(){
    return ui->lineEdit_value->text();

}
QString EnvEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString EnvEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}
