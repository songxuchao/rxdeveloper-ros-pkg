#include "ui_parameterEdit.h"
#include "parameterEdit.h"
#include <QMessageBox>

ParameterEdit::ParameterEdit(ParameterItem *item, QWidget *parent) : QDialog(parent), ui(new Ui::ParameterEdit) {
    ui->setupUi(this);
    param_type =item->getStandardParameter();
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
    }else {
        on_radioButton_rosparam_clicked();
        ui->lineEdit_param->setText(item->getName());
        ui->lineEdit_paramFile->setText(item->getValue());
        ui->lineEdit_namespace->setText(item->getNamespace());
        if (item->getType() == "command delete"){
            ui->comboBox_paramFile->setCurrentIndex(2);
        }else if (item->getType() == "command dump"){
            ui->comboBox_paramFile->setCurrentIndex(1);
        }else
            ui->comboBox_paramFile->setCurrentIndex(0);
    }
    ui->lineEdit_name->setFocus();
}

ParameterEdit::~ParameterEdit() {
    delete ui;
}

void ParameterEdit::reject() {
    ui->lineEdit_name->setText("");
    QDialog::reject();
}

void ParameterEdit::accept() {


    if ( (param_type==3 && (ui->lineEdit_paramFile->text()=="")) ||
            (param_type<3 && (ui->lineEdit_name->text()==""
                           || (ui->lineEdit_value->text()=="" && ui->lineEdit_file->text()=="" && ui->lineEdit_paramFile->text()=="")))  ){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Name or value missing</h2>"
                                                   "<p>Please insert the correct name and value!</p>"));
        ui->lineEdit_name->setFocus();

    } else{

        QDialog::accept();
    }

}

QString ParameterEdit::getName(){
    if (param_type==3)
        return ui->lineEdit_param->text();
    else
        return ui->lineEdit_name->text();
}
QString ParameterEdit::getValue(){
    if (param_type==1)
        return ui->lineEdit_value->text();
    else if (param_type==2)
        return ui->lineEdit_file->text();
    else
        return ui->lineEdit_paramFile->text();
}
QString ParameterEdit::getType(){
    if (param_type==1)
        return ui->lineEdit_type->text();
    else if (param_type==2)
        return ui->comboBox_file->itemText(ui->comboBox_file->currentIndex());
    else
        return ui->comboBox_paramFile->itemText(ui->comboBox_paramFile->currentIndex());
}
QString ParameterEdit::getNamespace(){
    return ui->lineEdit_namespace->text();
}


void ParameterEdit::on_radioButton_standard_clicked()
{
    ui->radioButton_standard->setChecked(true);
    ui->radioButton_file->setChecked(false);
    ui->radioButton_rosparam->setChecked(false);
    ui->groupBox_standard->setEnabled(true);
    ui->groupBox_file->setEnabled(false);
    ui->groupBox_rosparam->setEnabled(false);
    ui->lineEdit_name->setEnabled(true);
    param_type=1;
}


void ParameterEdit::on_radioButton_file_clicked()
{
    ui->radioButton_standard->setChecked(false);
    ui->radioButton_file->setChecked(true);
    ui->radioButton_rosparam->setChecked(false);
    ui->groupBox_standard->setEnabled(false);
    ui->groupBox_file->setEnabled(true);
    ui->groupBox_rosparam->setEnabled(false);
    ui->lineEdit_name->setEnabled(true);
    param_type=2;
}



void ParameterEdit::on_radioButton_rosparam_clicked()
{
    ui->radioButton_standard->setChecked(false);
    ui->radioButton_file->setChecked(false);
    ui->radioButton_rosparam->setChecked(true);
    ui->groupBox_standard->setEnabled(false);
    ui->groupBox_file->setEnabled(false);
    ui->groupBox_rosparam->setEnabled(true);
    ui->lineEdit_name->setEnabled(false);
    param_type=3;
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
