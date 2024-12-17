#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dockWidgetContents->setLayout(ui->controlPanelMasterLayout);
    ui->centralwidget->setLayout(ui->centralMasterLayout);

    ui->rulesTab->setLayout(ui->rulesTabLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupRuleEditor()
{
    // Initialize QsciScintilla editor
    editor = new QsciScintilla(this);

    // Set lexer for Python
    QsciLexerPython *lexer = new QsciLexerPython();
    editor->setLexer(lexer);

    // Set auto-completion
    editor->setAutoCompletionSource(QsciScintilla::AcsAll);
    editor->setAutoCompletionCaseSensitivity(false);
    editor->setAutoCompletionReplaceWord(true);
    editor->setAutoCompletionUseSingle(QsciScintilla::AcusAlways);

    // Set other editor properties
    editor->setUtf8(true);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    editor->setTabWidth(4);

    QVBoxLayout *layout = new QVBoxLayout(ui->rulesEditor);
    layout->addWidget(editor);
    ui->rulesEditor->setLayout(layout);
}
