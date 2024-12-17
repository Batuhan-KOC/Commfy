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
    // Set warning alarm icon and text visibility
    void SetSaveNeededWarningVisible(bool visible);

private slots:
    // Update ui elements from session's settings
    void UpdateSettingsWidgetsOnUI();

    void OpenRecentFile();

    // Invoked by create new session action to open a create session dialog
    void CreateNewSessionActionTriggered();

    // Invoked by open session action to open a file dialog to choose a commfy file
    void OpenSessionActionTriggered();

    // Create new session dialog has returned a name and location. Create a new session based on these values
    void NewSessionNameAndLocationChosen(QString sessionName, QString sessionLocation);

    // Send save session request to session manager
    void SaveSessionActionTriggered();

    //*******************************
    // Session Parameter Change Slots
    //*******************************

    // Session type option is updated on the ui
    void ProtocolTypeChanged(int index);
    void ToIpChanged(QString value);
    void ToPortChanged(int value);
    void FromIpChanged(QString value);
    void FromPortChanged(int value);
    void TxValueChanged(Qt::CheckState value);
    void RxValueChanged(Qt::CheckState value);
    void SerialDeviceChanged(QString value);
    void BaudrateChanged(int index);
    void FlowControlChanged(int index);
    void ParityChanged(int index);
    void StopBitsChanged(int index);
    void ByteSizeChanged(int index);
};
#endif // MAINWINDOW_H
