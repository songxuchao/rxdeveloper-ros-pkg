#include "ui_groupEdit.h"
#include "groupEdit.h"
#include <QMessageBox>

GroupEdit::GroupEdit(GroupItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::GroupEdit) {
    ui->setupUi(this);
    ui->lineEdit_namespace->setText(item->getNamespace());
    ui->comboBox_clear_params->setCurrentIndex(item->getClear_params());
    ui->spinBox_width->setValue(item->getWidth());
    ui->spinBox_height->setValue(item->getHeight());
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    ui->lineEdit_namespace->setFocus();
}

GroupEdit::~GroupEdit() {
    delete ui;
}

void GroupEdit::reject() {

    QDialog::reject();
}

void GroupEdit::accept() {

    if ( ui->comboBox_clear_params->currentIndex() != 0 && ui->lineEdit_namespace->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>No namespace specified</h2>"
                                                   "<p>clear_params can't be specified without namespace!</p>"));
        ui->lineEdit_namespace->setFocus();

    } else{

        QDialog::accept();
    }

}

QString GroupEdit::getNamespace(){
    return ui->lineEdit_namespace->text();

}
int GroupEdit::getClear_params()
{
    return ui->comboBox_clear_params->currentIndex();
}

int GroupEdit::getWidth()
{
    return ui->spinBox_width->value();
}

int GroupEdit::getHeight()
{
    return ui->spinBox_height->value();
}
QString GroupEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString GroupEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}
