#include "ui_newEntry.h"
#include "newEntry.h"

/*!\brief small dialog for naming.
 *
 * ...
 */
NewEntry::NewEntry(QWidget *parent) : QDialog(parent), ui(new Ui::NewEntry) {
    ui->setupUi(this);
}

NewEntry::~NewEntry() {
    delete ui;
}

void NewEntry::reject() {
    QDialog::reject();
}

void NewEntry::accept() {


    if (ui->lineEdit_newFile->text()==""){
        QMessageBox::information(this, (QString::fromUtf8("Information")),
                                 QString::fromUtf8("<h2>Name missing</h2>"
                                                   "<p>Please insert a correct name!</p>"));
        ui->lineEdit_newFile->setFocus();

    } else{
        QDialog::accept();
    }

}
QString NewEntry::getFileName(){
    return ui->lineEdit_newFile->text();
}


