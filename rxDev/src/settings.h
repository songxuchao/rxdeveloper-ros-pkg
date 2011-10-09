
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>


namespace Ui {
    class Settings;
}

class Settings : public QDialog {
        Q_OBJECT

        public:
                explicit Settings(QString work,QWidget *parent = 0);
                ~Settings();
                QString getWorkingDir();

        public slots:
                void reject();
                void accept();

        private slots:

        private:
                Ui::Settings *ui;

private slots:
                void on_pushButton_browseWorkingDir_clicked();

};

#endif
