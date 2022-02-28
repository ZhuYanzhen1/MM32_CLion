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

void MainWindow::on_variable_list_clicked(const QModelIndex &index) {
    for(int counter = 0; counter < index.model()->rowCount(); counter++) {
        QStandardItem *item = variable_list_table->item(counter, 0);
        qDebug() << item->text() << item->checkState();
    }
}

void MainWindow::on_clear_btn_clicked() {
    ui->debug_info_txt->clear();
    clear_debugger_variable();
    variable_list_table->clear();
    variable_list_table->setColumnCount(4);
    variable_list_table->setRowCount(0);
    variable_list_table->setHeaderData(0,Qt::Horizontal,"name");
    variable_list_table->setHeaderData(1,Qt::Horizontal,"type");
    variable_list_table->setHeaderData(2,Qt::Horizontal,"value");
    variable_list_table->setHeaderData(3,Qt::Horizontal,"address");
    for(int i = 0; i < 32; i++) {
        variable_list_item[i]->setCheckable(true);
        variable_list_item[i]->setCheckState(Qt::Unchecked);
    }
    ui->variable_list->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->variable_list->setModel(variable_list_table);
    ui->variable_list->setColumnWidth(0, 90);
    ui->variable_list->setColumnWidth(1, 55);
    ui->variable_list->setColumnWidth(2, 90);
    ui->variable_list->setColumnWidth(3, 100);
}
