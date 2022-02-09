#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QString>

static unsigned char variable_counter_serial = 0;

void MainWindow::mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
//    QString debug_string;
    const unsigned char array_buffer[9] = {data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 0x00};
//    debug_string = "Received PID:" + QString::number(pid) + ", 0x" + QString::number(data[0], 16)
//             + " 0x" + QString::number(data[1], 16) + " 0x" + QString::number(data[2], 16)
//             + " 0x" + QString::number(data[3], 16) + " 0x" + QString::number(data[4], 16)
//             + " 0x" + QString::number(data[5], 16) + " 0x" + QString::number(data[6], 16) + " 0x" + QString::number(data[7], 16);
//    qDebug() << debug_string;
    switch (pid) {
        case 0:
            if(data[0] == variable_counter_serial) {
                debugger_variable[variable_counter_serial]->var_status = data[1] << 16;
                debugger_variable[variable_counter_serial]->var_status |= data[2] << 8;
                debugger_variable[variable_counter_serial]->var_status |= data[3];
                debugger_variable[variable_counter_serial]->var_address = data[4] << 24;
                debugger_variable[variable_counter_serial]->var_address |= data[5] << 16;
                debugger_variable[variable_counter_serial]->var_address |= data[6] << 8;
                debugger_variable[variable_counter_serial]->var_address |= data[7];
            }
            break;
        case 1:
            if(data[0] == variable_counter_serial) {
                for(unsigned char counter = 0; counter < 7; counter++)
                    debugger_variable[variable_counter_serial]->var_name[counter] = data[counter + 1];
            }
            break;
        case 2:
            if(data[0] == variable_counter_serial) {
                for(unsigned char counter = 0; counter < 7; counter++)
                    debugger_variable[variable_counter_serial]->var_name[counter + 7] = data[counter + 1];
                debugger_variable[variable_counter_serial]->var_name[15] = 0x00;
                table_append_variable(debugger_variable[variable_counter_serial]->var_status,
                                      debugger_variable[variable_counter_serial]->var_value,
                                      debugger_variable[variable_counter_serial]->var_name,
                                      debugger_variable[variable_counter_serial]->var_address);
                variable_counter_serial++;
            }
            break;
        case 3:
            if(variable_counter_serial > data[0]) {
                debugger_variable[data[0]]->var_value = data[4] << 24;
                debugger_variable[data[0]]->var_value |= data[5] << 16;
                debugger_variable[data[0]]->var_value |= data[6] << 8;
                debugger_variable[data[0]]->var_value |= data[7];
                table_setvalue_variable(data[0], debugger_variable[data[0]]->var_value);
            }
            break;
        case 4:
            ui->debug_info_txt->setText(ui->debug_info_txt->toPlainText() + QString::fromLocal8Bit((const char *)array_buffer));
            break;
        default:
            break;
    }
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

void MainWindow::setup_serial_wire(QString baudrate) {
    serial = new QSerialPort();
    ui->serial_baudrate_txt->setText(baudrate);
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
