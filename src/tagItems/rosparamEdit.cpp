#include "ui_rosparamEdit.h"
#include "rosparamEdit.h"
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>

RosparamEdit::RosparamEdit(RosparamItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::RosparamEdit) {
    ui->setupUi(this);

    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    ui->lineEdit_param->setText(item->getName());
    ui->lineEdit_paramFile->setText(item->getValue());
    ui->lineEdit_namespace->setText(item->getNamespace());
    if (item->getType() == "command delete"){
        ui->comboBox_paramFile->setCurrentIndex(2);
    }else if (item->getType() == "command dump"){
        ui->comboBox_paramFile->setCurrentIndex(1);
    }else
        ui->comboBox_paramFile->setCurrentIndex(0);

    ui->lineEdit_paramFile->setFocus();
}

RosparamEdit::~RosparamEdit() {
    delete ui;
}

void RosparamEdit::reject() {
    ui->lineEdit_paramFile->setText("");
    QDialog::reject();
}

void RosparamEdit::accept() {


    if ( ui->lineEdit_paramFile->text()=="" && ui->lineEdit_param->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>File or param missing</h2>"
                                                   "<p>Please insert the correct file or param!</p>"));
        ui->lineEdit_paramFile->setFocus();

    } else{

        QDialog::accept();
    }

}

QString RosparamEdit::getName(){
    return ui->lineEdit_param->text();
}
QString RosparamEdit::getValue(){
    return ui->lineEdit_paramFile->text();
}
QString RosparamEdit::getType(){
    return ui->comboBox_paramFile->itemText(ui->comboBox_paramFile->currentIndex());
}
QString RosparamEdit::getNamespace(){
    return ui->lineEdit_namespace->text();
}

QString RosparamEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString RosparamEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}

void RosparamEdit::on_pushButton_tryOpen_clicked()
{
    QString file = ui->lineEdit_paramFile->text();
    if (file.startsWith("$(find ")){
        file.remove("$(find ");
        QStringList tempList;
        tempList<<file.split(")");
        QProcess findPath;
        findPath.start(QString("rospack find "+tempList.at(0)));
        findPath.waitForFinished(-1);
        QByteArray output = findPath.readAllStandardOutput();
        file = output.trimmed();
        file.append(tempList.at(1));
    }else{
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(file));

}
