#include "ui_wikiDialog.h"
#include "wikiDialog.h"
#include "QClipboard"

/*!\brief small dialog for naming.
 *
 * ...
 */
WikiDialog::WikiDialog(QWidget *parent) : QDialog(parent), ui(new Ui::WikiDialog) {
    ui->setupUi(this);
}

WikiDialog::~WikiDialog() {
    delete ui;
}

void WikiDialog::reject() {
    QDialog::reject();
}

void WikiDialog::accept() {
        QDialog::accept();
}

void WikiDialog::setText(QString text)
{
    ui->textEdit->setText(text);
}



void WikiDialog::on_pushButton_clipboard_clicked()
{
    QString text = ui->textEdit->toPlainText();
    QClipboard *cb = QApplication::clipboard();
    // set a text to the Clipboard
    cb->setText(text);
}
