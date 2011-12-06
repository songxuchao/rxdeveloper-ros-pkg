#include "ui_settings.h"
#include "settings.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

/*!\brief small dialog for naming.
 *
 * ...
 */
Settings::Settings(QString work,QString term,QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
    ui->setupUi(this);
    ui->lineEdit_workingDir->setText(work);
    ui->lineEdit_terminal->setText(term);
}

Settings::~Settings() {
    delete ui;
}

void Settings::reject() {
    QDialog::reject();
}

void Settings::accept() {

        if (ui->lineEdit_workingDir->text()==""){
            QMessageBox::information(this, (QString::fromUtf8("Information")),
                                     QString::fromUtf8("<h2>No path specified</h2>"
                                                       "<p>Please insert a correct path!</p>"));
            ui->lineEdit_workingDir->setFocus();
        }else
        QDialog::accept();

}
QString Settings::getWorkingDir(){
    return ui->lineEdit_workingDir->text();
}

/*!\brief working direcory dialog
 *
 * Launches the working directory dialog.
 */
void Settings::on_pushButton_browseWorkingDir_clicked()
{
    QString sourceFolder = QFileDialog::getExistingDirectory(this, tr("Choose working directory"), QDir::homePath());
    if (!sourceFolder.isEmpty())
        ui->lineEdit_workingDir->setText( sourceFolder);
}




void Settings::on_pushButton_default_clicked()
{
    ui->lineEdit_terminal->setText("xterm -hold -e bash -i -c");
}

QString Settings::getTerminal()
{
    return ui->lineEdit_terminal->text();
}
