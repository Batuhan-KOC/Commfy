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
    // Enable commfy subwidgets
    void SetEnableWidgets(bool enable);
    // Some widgets and layouts requires initializaiton. Performing them here
    void InitializeLayoutsAndOtherWidgets();

private slots:
    void OpenRecentFile();

    // Invoked by create new session action to open a create session dialog
    void CreateNewSessionActionTriggered();

    // Invoked by open session action to open a file dialog to choose a commfy file
    void OpenSessionActionTriggered();

    // Create new session dialog has returned a name and location. Create a new session based on these values
    void NewSessionNameAndLocationChosen(QString sessionName, QString sessionLocation);

    // Session type option is updated on the ui
    void SessionProtocolTypeUpdated(int index);
};
#endif // MAINWINDOW_H
