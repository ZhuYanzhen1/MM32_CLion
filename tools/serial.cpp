#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

void MainWindow::mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
    QString debug_string;
    (void) pid;
    (void) data;
    debug_string = "Received PID:" + QString::number(pid) + ", 0x" + QString::number(data[0], 16)
             + " 0x" + QString::number(data[1], 16) + " 0x" + QString::number(data[2], 16)
             + " 0x" + QString::number(data[3], 16) + " 0x" + QString::number(data[4], 16)
             + " 0x" + QString::number(data[5], 16) + " 0x" + QString::number(data[6], 16) + " 0x" + QString::number(data[7], 16);
    qDebug() << debug_string;
}

bool MainWindow::set_serial_badurate(void) {
    if(ui->serial_port_cb->currentText() == NULL || ui->serial_baudrate_txt->toPlainText() == NULL) {
        QMessageBox::information(NULL, "Error", "Please select port number and baudrate first!");
        return false;
    } else {
        serial->setPortName(ui->serial_port_cb->currentText());
        serial->setBaudRate(ui->serial_baudrate_txt->toPlainText().toInt());
        serial->setDataBits(QSerialPort::Data8);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        return true;
    }
}

void MainWindow::refresh_serial_port(void) {
    QList<QSerialPortInfo> serialPortinfo = QSerialPortInfo::availablePorts();
    int count = serialPortinfo.count();
    ui->serial_port_cb->clear();
    for(int i = 0; i < count; i++)
        ui->serial_port_cb->addItem(serialPortinfo.at(i).portName());
}

void MainWindow::serial_received(void) {
    QByteArray info = serial->readAll();
    char *buffer = info.data();
    for (int counter = 0; counter < info.length(); counter++)
        mdtp_receive_handler(buffer[counter]);
}
