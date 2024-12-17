#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Layout management
    ui->dockWidgetContents->setLayout(ui->controlPanelMasterLayout);
    ui->centralwidget->setLayout(ui->centralMasterLayout);
    ui->rulesTab->setLayout(ui->rulesTabLayout);
    ui->monitoringTab->setLayout(ui->monitoringTabLayout1);

    SetupRuleEditor();
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
