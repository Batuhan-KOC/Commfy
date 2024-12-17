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

    InitializeLayoutsAndOtherWidgets();

    SetupRuleEditor();

    InitializeSessionManager();

    // Connect menu actions
    connect(ui->actionCreateNewSession, SIGNAL(triggered()), this, SLOT(CreateNewSessionActionTriggered()));

    connect(ui->actionOpenSession, SIGNAL(triggered()), this, SLOT(OpenSessionActionTriggered()));
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
}

void MainWindow::OpenRecentFile()
{

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
        }
    }
}

void MainWindow::NewSessionNameAndLocationChosen(QString sessionName, QString sessionLocation)
{
    bool sessionCreatedSuccess = sessionManager->CreateNewSession(sessionName, sessionLocation);

    // If session creation is success, enable widgets
    SetEnableWidgets(sessionCreatedSuccess);

    // Enable the close and save actions if session creation is successfull
    ui->actionCloseSession->setEnabled(sessionCreatedSuccess);
    ui->actionSaveSession->setEnabled(sessionCreatedSuccess);
}
