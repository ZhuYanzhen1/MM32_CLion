#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "math.h"

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

//    table_append_variable(0, 10, "test1", 0x20000000);
//    table_append_variable(1, 100, "test2", 0x20000004);
//    table_append_variable(5, 1000, "test3", 0x20000008);
//    double timestamp = -5;
//    for(int counter = 0; counter < 1500; counter++) {
//        (*debugger_variable[0]).x->append(timestamp);
//        (*debugger_variable[0]).y->append(sin(timestamp));
//        if(counter > 700 && counter < 1300) {
//            (*debugger_variable[1]).x->append(timestamp);
//            (*debugger_variable[1]).y->append(sin(timestamp + 2.094395));
//            (*debugger_variable[2]).x->append(timestamp);
//            (*debugger_variable[2]).y->append(sin(timestamp + 4.188790));
//        }
//        timestamp = timestamp + 0.01;
//    }
//    ui->debug_info_txt->append("hello\r\nthis is a test text");
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
    unsigned char checked_list[12] = {0};
    int result = 0;
    for(int counter = 0; counter < index.model()->rowCount(); counter++) {
        QStandardItem *item = variable_list_table->item(counter, 0);
        checked_list[counter] = item->checkState() == Qt::CheckState::Checked ? 1 : 0;
    }
    for(int counter = 0; counter < 12; counter++)
        if(checked_list[counter] != checked_list_last[counter])
            result = checked_list[counter] == 1 ? counter + 1 : -1 * counter - 1;
    for(int copy_counter = 0; copy_counter < 12; copy_counter++)
        checked_list_last[copy_counter] = checked_list[copy_counter];

    if(result > 0) {
        plot_curve_list[plot_curve_counter] = result;
        QPen pen;
        pen.setColor(back_color_table[result - 1]);
        pen.setWidthF(3);
        ui->custom_plot->addGraph();
        ui->custom_plot->graph(plot_curve_counter)->setData(*debugger_variable[result - 1]->x, *debugger_variable[result - 1]->y);
        ui->custom_plot->graph(plot_curve_counter)->setPen(pen);
        ui->custom_plot->graph(plot_curve_counter)->setName(debugger_variable[result - 1]->var_name);
        ui->custom_plot->graph(plot_curve_counter)->rescaleAxes();
        ui->custom_plot->replot();
        plot_curve_counter++;
    } else if (result != 0) {
        int plot_curve_index = 0;
        for(int counter = 0; counter < 12; counter++)
            if(plot_curve_list[counter] == (-1 * result))
                plot_curve_index = counter;
        ui->custom_plot->removeGraph(plot_curve_index);
        ui->custom_plot->replot();
        for(int counter = plot_curve_index; counter < 11; counter++)
            plot_curve_list[counter] = plot_curve_list[counter + 1];
        plot_curve_counter--;
    }
}

void MainWindow::on_clear_btn_clicked() {
    ui->debug_info_txt->clear();
//    clear_debugger_variable();     // 这一步也可能会导致程序崩溃
//    clear_table_variable();     // 这一步可能会导致程序崩溃
}
