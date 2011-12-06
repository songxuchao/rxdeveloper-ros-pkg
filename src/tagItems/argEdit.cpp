#include "ui_argEdit.h"
#include "argEdit.h"
#include <QMessageBox>

ArgEdit::ArgEdit(ArgItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::ArgEdit) {
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit_value_or_default->setEnabled(false);
    ui->lineEdit_name->setText(item->getName());
    ui->lineEdit_value_or_default->setText(item->getValue());
    ui->comboBox->setCurrentIndex(item->getValue_or_default());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    ui->lineEdit_name->setFocus();
}

ArgEdit::~ArgEdit() {
    delete ui;
}

void ArgEdit::reject() {

    QDialog::reject();
}

void ArgEdit::accept() {

    if ( ui->lineEdit_name->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No name specified</h2>"
                                                   "<p>Please insert the correct name!</p>"));
        ui->lineEdit_name->setFocus();

    } else{

        QDialog::accept();
    }

}

QString ArgEdit::getName(){
    return ui->lineEdit_name->text();
}
QString ArgEdit::getValue(){
    return ui->lineEdit_value_or_default->text();

}


int ArgEdit::getValue_or_default()
{
    return (ui->comboBox->currentIndex());
}

void ArgEdit::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0){
        ui->lineEdit_value_or_default->setText("");
        ui->lineEdit_value_or_default->setToolTip("none");
        ui->lineEdit_value_or_default->setEnabled(false);
    } else if (index == 1){
        ui->lineEdit_value_or_default->setToolTip("bar");
        ui->lineEdit_value_or_default->setEnabled(true);
    } else if (index == 2){
        ui->lineEdit_value_or_default->setToolTip("1");
        ui->lineEdit_value_or_default->setEnabled(true);
    }
}

QString ArgEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString ArgEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}
