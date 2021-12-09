#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    serial = new QSerialPort();
    ui->setupUi(this);
    refresh_serial_port();
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

void MainWindow::on_open_serial_btn_clicked() {
    if(serial->isOpen() == false){
        if(set_serial_badurate() == true) {
            serial->open(QSerialPort::ReadWrite);
            connect(serial, SIGNAL(readyRead()), this, SLOT(serial_received()));
            ui->open_serial_btn->setText("Close");
        }
    } else {
        serial->close();
        ui->open_serial_btn->setText("Open");
    }
}
