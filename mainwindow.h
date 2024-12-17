#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/session/sessionmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SessionManager* sessionManager;

    QAction* recentSessionsList[MAXIMUM_RECENT_FILE_LIMIT] = {nullptr};

    // Setup the rule editor in the tab widget with qsciscintilla widget
    void SetupRuleEditor();
    // Initialize session manager and get the recent session locations
    void InitializeSessionManager();

private slots:
    void OpenRecentFile();

    // Invoked by create new session action to open a create session dialog
    void CreateNewSessionActionTriggered();

    // Create new session dialog has returned a name and location. Create a new session based on these values
    void NewSessionNameAndLocationChosen(QString sessionName, QString sessionLocation);
};
#endif // MAINWINDOW_H
