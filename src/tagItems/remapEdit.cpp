#include "ui_remapEdit.h"
#include "remapEdit.h"
#include <QMessageBox>

RemapEdit::RemapEdit(RemapItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::RemapEdit) {
    ui->setupUi(this);
    ui->lineEdit_from->setText(item->getFrom());
    ui->lineEdit_to->setText(item->getTo());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());

}

RemapEdit::~RemapEdit() {
    delete ui;
}

void RemapEdit::reject() {
    QDialog::reject();
}

void RemapEdit::accept() {

    if ( ui->lineEdit_from->text()=="" || ui->lineEdit_to->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No original-name or new-name specified</h2>"
                                                   "<p>Please insert the correct 'from' and 'to' value!</p>"));
        ui->lineEdit_from->setFocus();

    } else{

        QDialog::accept();
    }

}

QString RemapEdit::getFrom(){
    return ui->lineEdit_from->text();
}
QString RemapEdit::getTo(){
    return ui->lineEdit_to->text();

}
QString RemapEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString RemapEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}
QString RemapItem::getIf()
{
    return _ifString;
}

void RemapItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString RemapItem::getUnless()
{
    return _unlessString;
}

void RemapItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}
