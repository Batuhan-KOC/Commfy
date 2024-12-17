#include "createnewsessiondialog.h"
#include "ui_createnewsessiondialog.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFileInfo>

CreateNewSessionDialog::CreateNewSessionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateNewSessionDialog)
{
    ui->setupUi(this);

    setLayout(ui->createNewSessionLayout);

    connect(ui->locationChooseButton, SIGNAL(clicked()), this, SLOT(OpenFolderDialog()));
}

CreateNewSessionDialog::~CreateNewSessionDialog()
{
    delete ui;
}

void CreateNewSessionDialog::on_buttonBox_accepted()
{
    if(ui->sessionNameEdit->text().isEmpty()){
        QMessageBox::warning(this, tr("Commfy"), tr("Session name can not be empty."), QMessageBox::Ok);
        return;
    }
    else if(ui->locationEdit->text().isEmpty()){
        QMessageBox::warning(this, tr("Commfy"), tr("Session location can not be empty."), QMessageBox::Ok);
        return;
    }

    QFileInfo sessionFileInfo(ui->locationEdit->text());

    if(!sessionFileInfo.exists()){
        QMessageBox::warning(this, tr("Commfy"), tr("Session location does not exist."), QMessageBox::Ok);
        return;
    }
    else if(!sessionFileInfo.isDir()){
        QMessageBox::warning(this, tr("Commfy"), tr("Session location is not a directory."), QMessageBox::Ok);
        return;
    }
    else{
        emit NewSessionNameAndLocationChosen(ui->sessionNameEdit->text(), ui->locationEdit->text());

        close();
    }
}


void CreateNewSessionDialog::on_buttonBox_rejected()
{
    close();
}

void CreateNewSessionDialog::OpenFolderDialog()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty()){
        ui->locationEdit->setText(dir);
    }
}

