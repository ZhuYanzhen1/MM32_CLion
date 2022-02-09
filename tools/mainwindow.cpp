#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    refresh_serial_port();
    setup_serial_wire("115200");
    setup_variable_table();
    setup_custom_plot();
}

MainWindow::~MainWindow() {
    if (serial->isOpen())
        serial->close();
    delete serial;
    delete ui;
}

void MainWindow::on_open_serial_btn_clicked() {
    if(serial->isOpen() == false && set_serial_badurate() == true) {
        serial->open(QSerialPort::ReadWrite);
        connect(serial, SIGNAL(readyRead()), this, SLOT(serial_received()));
        ui->open_serial_btn->setText("Close");
    } else {
        serial->close();
        ui->open_serial_btn->setText("Open");
    }
}

void MainWindow::on_refresh_serial_btn_clicked() {
    refresh_serial_port();
}
