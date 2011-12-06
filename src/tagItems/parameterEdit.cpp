#include "ui_parameterEdit.h"
#include "parameterEdit.h"
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>


ParameterEdit::ParameterEdit(ParameterItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::ParameterEdit) {
    ui->setupUi(this);
    param_type =item->getStandardParameter();
    //determine range for dyn reconfigure params
    qDebug()<<item->getRange();
    if (item->getRange()==""){
        range_high= 0;
        range_low = 0;
    }else{
        QStringList range = (item->getRange()).split(",");
        if (range.size()>1){
            QString low = range.at(0);
            QString high = range.at(1);
            low.remove(0,1);
            high.chop(1);
            qDebug()<<high<<" "<<low;
            range_low=low.toDouble();
            range_high=high.toDouble();
            if (range_high < range_low){
                double temp;
                temp = range_high;
                range_high= range_low;
                range_low = temp;
            }
        }
    }
    ui->lineEdit_if->setText(item->getIf());
    ui->lineEdit_unless->setText(item->getUnless());
    if (param_type==1){
        on_radioButton_standard_clicked();
        ui->lineEdit_name->setText(item->getName());
        ui->lineEdit_value->setText(item->getValue());
        ui->lineEdit_type->setText(item->getType());
    }else if (param_type==2){
        on_radioButton_file_clicked();
        ui->lineEdit_name->setText(item->getName());
        ui->lineEdit_file->setText(item->getValue());

        if (item->getType() == "command"){
            ui->comboBox_file->setCurrentIndex(2);
        }else if (item->getType() == "binfile"){
            ui->comboBox_file->setCurrentIndex(1);
        }else
            ui->comboBox_file->setCurrentIndex(0);
    }
    ui->lineEdit_name->setFocus();
}

ParameterEdit::~ParameterEdit() {
    delete ui;
}

void ParameterEdit::reject() {
    QDialog::reject();
}

void ParameterEdit::accept() {


    if (  (ui->lineEdit_name->text()=="" || (ui->lineEdit_value->text()=="" && ui->lineEdit_file->text()=="")))  {
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Name or value missing</h2>"
                                                   "<p>Please insert the correct name and value!</p>"));
        ui->lineEdit_name->setFocus();

    } else{
        if (range_low!=range_high){
            double value=ui->lineEdit_value->text().toDouble();
            if (value>range_high||value<range_low){
                QString text = QString("<h2>Parameter out of range</h2>").append(QString("<p>The value of parameter %1 should be between %2 and %3!</p>").arg(getName()).arg(range_low).arg(range_high));
                QMessageBox::warning(this, (QString::fromUtf8("Warning")),
                                     (text));
            }

        }

        QDialog::accept();


    }}

QString ParameterEdit::getName(){
    return ui->lineEdit_name->text();
}
QString ParameterEdit::getValue(){
    if (param_type==1)
        return ui->lineEdit_value->text();
    else
        return ui->lineEdit_file->text();
}
QString ParameterEdit::getType(){
    if (param_type==1)
        return ui->lineEdit_type->text();
    else
        return ui->comboBox_file->itemText(ui->comboBox_file->currentIndex());
}

void ParameterEdit::on_radioButton_standard_clicked()
{
    ui->radioButton_standard->setChecked(true);
    ui->radioButton_file->setChecked(false);
    ui->groupBox_standard->setEnabled(true);
    ui->groupBox_file->setEnabled(false);
    ui->lineEdit_name->setEnabled(true);
    param_type=1;
}


void ParameterEdit::on_radioButton_file_clicked()
{
    ui->radioButton_standard->setChecked(false);
    ui->radioButton_file->setChecked(true);
    ui->groupBox_standard->setEnabled(false);
    ui->groupBox_file->setEnabled(true);
    ui->lineEdit_name->setEnabled(true);
    param_type=2;
}



int ParameterEdit::getParamType()
{
    return param_type;
}

QString ParameterEdit::getIf()
{
    return ui->lineEdit_if->text();
}

QString ParameterEdit::getUnless()
{
    return ui->lineEdit_unless->text();
}
void ParameterEdit::on_pushButton_tryOpen_clicked()
{
    QString file = ui->lineEdit_file->text();
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
