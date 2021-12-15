#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    serial = new QSerialPort();
    ui->setupUi(this);
    refresh_serial_port();
    ui->serial_baudrate_txt->setText("115200");

    variable_list_table = new QStandardItemModel();
    for(int i = 0; i < 32; i++)
       variable_list_item[i] = new QStandardItem();

    variable_list_table->setColumnCount(3);
    variable_list_table->setRowCount(1);
    variable_list_table->setHeaderData(0,Qt::Horizontal," ");
    variable_list_table->setHeaderData(1,Qt::Horizontal,"type");
    variable_list_table->setHeaderData(2,Qt::Horizontal,"name");
    for(int i = 0; i < 32; i++) {
        variable_list_item[i]->setCheckable(true);
        variable_list_item[i]->setCheckState(Qt::Unchecked);
    }
    ui->variable_list->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->variable_list->setModel(variable_list_table);
    ui->variable_list->setColumnWidth(0, 20);
    ui->variable_list->setColumnWidth(1, 55);
    ui->variable_list->setColumnWidth(2, 80);

    variable_list_table->setRowCount(1);
    variable_list_table->setItem(0, variable_list_item[0]);
    variable_list_table->setItem(0, 1, new QStandardItem("uint32"));
    variable_list_table->setItem(0, 2, new QStandardItem("counter"));
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
