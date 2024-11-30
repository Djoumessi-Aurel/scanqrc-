#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scannerWidget(new QRCodeScanner(this))
{
    setCentralWidget(scannerWidget);
    setWindowTitle("Scanner de QR Code");
    resize(800, 600);
    setFixedHeight(600);
}

MainWindow::~MainWindow()
{
}

