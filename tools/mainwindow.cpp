#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
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

static unsigned char plot_curve_counter = 0;
static unsigned char checked_list[12] = {0};
static unsigned char checked_counter = 0;
void MainWindow::on_variable_list_clicked(const QModelIndex &index) {
    plot_curve_counter = 0;
    checked_counter = 0;
    for(int counter = 0; counter < index.model()->rowCount(); counter++) {
        QStandardItem *item = variable_list_table->item(counter, 0);
        if(item->checkState() == Qt::CheckState::Checked) {
//            if(checked_list[checked_counter] != counter)        // 这次操作后删掉的元素
            checked_list[checked_counter] = counter;
            checked_counter++;
//            QPen pen;
//            pen.setColor(color_table[counter]);
//            ui->custom_plot->addGraph();
//            // 添加、删除graph还没有做
//            ui->custom_plot->graph(plot_curve_counter)->setData(*debugger_variable[counter]->x, *debugger_variable[counter]->y);
//            ui->custom_plot->graph(plot_curve_counter)->setPen(pen);
//            ui->custom_plot->graph(plot_curve_counter)->setName(debugger_variable[counter]->var_name);
//            plot_curve_counter++;
        }
    }
}

void MainWindow::on_clear_btn_clicked() {
    ui->debug_info_txt->clear();
    clear_debugger_variable();     // 这一步也可能会导致程序崩溃
    clear_table_variable();     // 这一步可能会导致程序崩溃
}
