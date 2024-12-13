#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dockWidgetContents->setLayout(ui->controlPanelMasterLayout);
    ui->centralwidget->setLayout(ui->centralMasterLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
