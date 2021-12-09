#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    serial = new QSerialPort();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    if (serial->isOpen())
        serial->close();
    delete serial;
    delete ui;
}

void MainWindow::mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
    (void) pid;
    (void) data;
}
