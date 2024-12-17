#ifndef CREATENEWSESSIONDIALOG_H
#define CREATENEWSESSIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateNewSessionDialog;
}

class CreateNewSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewSessionDialog(QWidget *parent = nullptr);
    ~CreateNewSessionDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void OpenFolderDialog();

signals:
    void NewSessionNameAndLocationChosen(QString sessionName, QString sessionLocation);

private:
    Ui::CreateNewSessionDialog *ui;
};

#endif // CREATENEWSESSIONDIALOG_H
