#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <Qsci/qscilexerpython.h>

#include "src/ui/createnewsessiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set all the widgets on the screen disabled at first
    SetEnableWidgets(false);

    // No save needed at startup
    SetSaveNeededWarningVisible(false);

    InitializeLayoutsAndOtherWidgets();

    SetupRuleEditor();

    InitializeSessionManager();

    // Connect menu actions
    connect(ui->actionCreateNewSession, SIGNAL(triggered()), this, SLOT(CreateNewSessionActionTriggered()));
    connect(ui->actionOpenSession, SIGNAL(triggered()), this, SLOT(OpenSessionActionTriggered()));
    connect(ui->actionSaveSession, SIGNAL(triggered()), this, SLOT(SaveSessionActionTriggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupRuleEditor()
{
    // Set lexer for Python
    QsciLexerPython *lexer = new QsciLexerPython();
    ui->rulesEditor->setLexer(lexer);

    // Set auto-completion
    ui->rulesEditor->setAutoCompletionSource(QsciScintilla::AcsAll);
    ui->rulesEditor->setAutoCompletionCaseSensitivity(false);
    ui->rulesEditor->setAutoCompletionReplaceWord(true);
    ui->rulesEditor->setAutoCompletionUseSingle(QsciScintilla::AcusAlways);

    // Set other editor properties
    ui->rulesEditor->setUtf8(true);
    ui->rulesEditor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    ui->rulesEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    ui->rulesEditor->setTabWidth(4);
}

void MainWindow::InitializeSessionManager()
{
    sessionManager = new SessionManager(this);

    connect(sessionManager, SIGNAL(UpdateSettingsWidgetsOnUI()), this, SLOT(UpdateSettingsWidgetsOnUI()));

    QStringList recentSessions = sessionManager->ReadSessionsFile();

    // If there are recent sessions, list them on the ui
    if(recentSessions.size() > 0){
        int sessionIndex = 0;

        for(const QString &sessionFileName: recentSessions){
            QAction* sessionAction = new QAction(this);

            sessionAction->setText(sessionFileName);

            recentSessionsList[sessionIndex] = sessionAction;

            connect(sessionAction, SIGNAL(triggered()), this, SLOT(OpenRecentFile()));

            ui->menuOpenRecent->addAction(sessionAction);

            sessionIndex++;

            if(sessionIndex >= MAXIMUM_RECENT_FILE_LIMIT){
                break;
            }
        }
    }
    // If there is no recent session, disable the open recent session menu
    else{
        ui->menuOpenRecent->setEnabled(false);
    }

}

void MainWindow::SetEnableWidgets(bool enable)
{
    ui->controlPanelContents->setEnabled(enable);
    ui->centralTabWidget->setEnabled(enable);
}

void MainWindow::InitializeLayoutsAndOtherWidgets()
{
    ui->controlPanelContents->setLayout(ui->controlPanelMasterLayout);
    ui->centralwidget->setLayout(ui->centralMasterLayout);
    ui->rulesTab->setLayout(ui->rulesTabLayout);
    ui->monitoringTab->setLayout(ui->monitoringTabLayout1);

    connect(ui->protocolSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(ProtocolTypeChanged(int)));
    connect(ui->toIp, SIGNAL(textEdited(QString)), this, SLOT(ToIpChanged(QString)));
    connect(ui->toPort, SIGNAL(valueChanged(int)), this, SLOT(ToPortChanged(int)));
    connect(ui->fromIp, SIGNAL(textEdited(QString)), this, SLOT(FromIpChanged(QString)));
    connect(ui->fromPort, SIGNAL(valueChanged(int)), this, SLOT(FromPortChanged(int)));
    connect(ui->txCheckbox, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(TxValueChanged(Qt::CheckState)));
    connect(ui->rxCheckbox, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(RxValueChanged(Qt::CheckState)));
    connect(ui->serialDeviceCombobox, SIGNAL(currentTextChanged(QString)), this, SLOT(SerialDeviceChanged(QString)));
    connect(ui->baudrateCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudrateChanged(int)));
    connect(ui->flowControlCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(FlowControlChanged(int)));
    connect(ui->parityCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(ParityChanged(int)));
    connect(ui->stopBitCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(StopBitsChanged(int)));
    connect(ui->byteSizeCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(ByteSizeChanged(int)));
}

void MainWindow::SetSaveNeededWarningVisible(bool visible)
{
    ui->alarmIcon->setVisible(visible);
    ui->saveNeededLabel->setVisible(visible);
}

void MainWindow::UpdateSettingsWidgetsOnUI()
{
    bool ok = false;

    SessionSettings settings = sessionManager->GetActiveSettings(ok);

    if(ok){
        ui->protocolSelection->setCurrentIndex(static_cast<int>(settings.protocol));
        ui->toIp->setText(settings.toIp);
        ui->toPort->setValue(settings.toPort);
        ui->fromIp->setText(settings.fromIp);
        ui->fromPort->setValue(settings.fromPort);
        ui->txCheckbox->setChecked(settings.txEnabled);
        ui->rxCheckbox->setChecked(settings.rxEnabled);
        if(!settings.serialDevice.isEmpty()){
            ui->serialDeviceCombobox->addItem(settings.serialDevice);
            ui->serialDeviceCombobox->setCurrentText(settings.serialDevice);
        }
        ui->baudrateCombobox->setCurrentIndex(static_cast<int>(settings.baudrate));
        ui->flowControlCombobox->setCurrentIndex(static_cast<int>(settings.flowControl));
        ui->parityCombobox->setCurrentIndex(static_cast<int>(settings.parity));
        ui->stopBitCombobox->setCurrentIndex(static_cast<int>(settings.stopBits));
        ui->byteSizeCombobox->setCurrentIndex(static_cast<int>(settings.byteSize));

        SetSaveNeededWarningVisible(false);
    }
}

void MainWindow::OpenRecentFile()
{
    // Get the pointer to the sender object
    QObject* senderObj = sender();

    // Check if the sender is a QAction
    QAction* action = qobject_cast<QAction*>(senderObj);
    if (action) {
        // Get the text of the QAction which is the file location of the session
        QString sessionFileLocation = action->text();

        bool result = sessionManager->OpenSession(sessionFileLocation);

        // Enable widgets if session is opened successfully
        SetEnableWidgets(result);

        ui->actionSaveSession->setEnabled(result);
        ui->actionCloseSession->setEnabled(result);

        if(result){
            bool ok = false;

            SessionProtocolType currentProtocol = sessionManager->GetActiveSettings(ok).protocol;

            if(ok){
                ProtocolTypeChanged(static_cast<int>(currentProtocol));
            }
        }
    }
}

void MainWindow::CreateNewSessionActionTriggered()
{
    CreateNewSessionDialog dialog;

    connect(&dialog, SIGNAL(NewSessionNameAndLocationChosen(QString,QString)), this, SLOT(NewSessionNameAndLocationChosen(QString,QString)));

    dialog.exec();

    disconnect(&dialog, SIGNAL(NewSessionNameAndLocationChosen(QString,QString)), this, SLOT(NewSessionNameAndLocationChosen(QString,QString)));
}

void MainWindow::OpenSessionActionTriggered()
{
    // Create a file dialog
    QFileDialog dialog(this, "Open Commfy File");

    // Set the file mode to existing file
    dialog.setFileMode(QFileDialog::ExistingFile);

    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    // Set the name filter for ".commfy" files
    dialog.setNameFilter("Commfy files (*.commfy)");

    if(dialog.exec() == QDialog::Accepted){
        QStringList selectedFiles = dialog.selectedFiles();

        if(selectedFiles.size() > 0){
            QString filePath = selectedFiles.first();

            bool result = sessionManager->OpenSession(filePath);

            // Enable widgets if session is opened successfully
            SetEnableWidgets(result);

            ui->actionSaveSession->setEnabled(result);
            ui->actionCloseSession->setEnabled(result);

            if(result){
                bool ok = false;

                SessionProtocolType currentProtocol = sessionManager->GetActiveSettings(ok).protocol;

                if(ok){
                    ProtocolTypeChanged(static_cast<int>(currentProtocol));
                }
            }
        }
    }
}

void MainWindow::NewSessionNameAndLocationChosen(QString sessionName, QString sessionLocation)
{
    bool sessionCreatedSuccess = sessionManager->CreateNewSession(sessionName, sessionLocation);

    // Zero is the index of the udp protocol. Which is the default protocol type
    ProtocolTypeChanged(0);

    // If session creation is success, enable widgets
    SetEnableWidgets(sessionCreatedSuccess);

    // Enable the close and save actions if session creation is successfull
    ui->actionCloseSession->setEnabled(sessionCreatedSuccess);
    ui->actionSaveSession->setEnabled(sessionCreatedSuccess);
}

void MainWindow::SaveSessionActionTriggered()
{
    sessionManager->SaveActiveSession();

    SetSaveNeededWarningVisible(false);
}

void MainWindow::ProtocolTypeChanged(int index)
{
    SessionProtocolType protocol = static_cast<SessionProtocolType>(index);

    ui->serialDeviceCombobox->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);
    ui->updateSerialDevicesList->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);
    ui->baudrateCombobox->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);
    ui->flowControlCombobox->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);
    ui->parityCombobox->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);
    ui->stopBitCombobox->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);
    ui->byteSizeCombobox->setEnabled(protocol == SessionProtocolType::PROTOCOL_SERIAL);

    ui->toIp->setEnabled(protocol == SessionProtocolType::PROTOCOL_UDP || protocol == SessionProtocolType::PROTOCOL_TCP);
    ui->toPort->setEnabled(protocol == SessionProtocolType::PROTOCOL_UDP || protocol == SessionProtocolType::PROTOCOL_TCP);
    ui->fromIp->setEnabled(protocol == SessionProtocolType::PROTOCOL_UDP || protocol == SessionProtocolType::PROTOCOL_TCP);
    ui->fromPort->setEnabled(protocol == SessionProtocolType::PROTOCOL_UDP || protocol == SessionProtocolType::PROTOCOL_TCP);
    ui->rxCheckbox->setEnabled(protocol == SessionProtocolType::PROTOCOL_UDP || protocol == SessionProtocolType::PROTOCOL_TCP);
    ui->txCheckbox->setEnabled(protocol == SessionProtocolType::PROTOCOL_UDP || protocol == SessionProtocolType::PROTOCOL_TCP);

    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_PROTOCOL_TYPE, QVariant::fromValue(protocol));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::ToIpChanged(QString value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_TO_IP, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::ToPortChanged(int value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_TO_PORT, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::FromIpChanged(QString value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_FROM_IP, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::FromPortChanged(int value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_FROM_PORT, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::TxValueChanged(Qt::CheckState value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_TX_ENABLED, QVariant::fromValue(value == Qt::Checked));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::RxValueChanged(Qt::CheckState value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_RX_ENABLED, QVariant::fromValue(value == Qt::Checked));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::SerialDeviceChanged(QString value)
{
    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_SERIAL_DEVICE, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::BaudrateChanged(int index)
{
    SessionBaudrate value = static_cast<SessionBaudrate>(index);

    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_SERIAL_DEVICE, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::FlowControlChanged(int index)
{
    SessionFlowControl value = static_cast<SessionFlowControl>(index);

    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_FLOW_CONTROL, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::ParityChanged(int index)
{
    SessionParity value = static_cast<SessionParity>(index);

    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_PARITY, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::StopBitsChanged(int index)
{
    SessionStopBits value = static_cast<SessionStopBits>(index);

    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_STOP_BITS, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}

void MainWindow::ByteSizeChanged(int index)
{
    SessionByteSize value = static_cast<SessionByteSize>(index);

    sessionManager->SessionSettingsValueChanged(SESSION_SETTINGS_BYTE_SIZE, QVariant::fromValue(value));

    SetSaveNeededWarningVisible(true);
}
