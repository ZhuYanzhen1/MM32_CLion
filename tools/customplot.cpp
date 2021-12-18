#include "mainwindow.h"
#include "./ui_mainwindow.h"

static const unsigned char signed_int8 = 0;
static const unsigned char signed_int16 = 1;
static const unsigned char signed_int32 = 2;
static const unsigned char unsigned_int8 = 3;
static const unsigned char unsigned_int16 = 4;
static const unsigned char unsigned_int32 = 5;
static const unsigned char float_32bit = 6;

void MainWindow::table_append_variable(unsigned char index, unsigned char type,
                                       unsigned int value, char *name, unsigned int address) {
    debugger_variable[index]->var_status = type;
    debugger_variable[index]->var_address = address;
    debugger_variable[index]->var_value = value;
    for(unsigned char counter = 0; counter < 14; counter++)
        debugger_variable[index]->var_name[counter] = name[counter];

    variable_list_table->setRowCount(index + 1);
    variable_list_table->setItem(index, variable_list_item[index]);
    switch(type) {
        case signed_int8:
            variable_list_table->setItem(index, 1, new QStandardItem("int8"));
        break;
        case signed_int16:
            variable_list_table->setItem(index, 1, new QStandardItem("int16"));
        break;
        case signed_int32:
            variable_list_table->setItem(index, 1, new QStandardItem("int32"));
        break;
        case unsigned_int8:
            variable_list_table->setItem(index, 1, new QStandardItem("uint8"));
        break;
        case unsigned_int16:
            variable_list_table->setItem(index, 1, new QStandardItem("uint16"));
        break;
        case unsigned_int32:
            variable_list_table->setItem(index, 1, new QStandardItem("uint32"));
        break;
        case float_32bit:
            variable_list_table->setItem(index, 1, new QStandardItem("float"));
        break;
    }
    variable_list_table->setItem(index, 2, new QStandardItem("155"));
    variable_list_item[index]->setBackground(Qt::green);
    variable_list_item[index]->setText(name);
}

void MainWindow::setup_variable_table(void) {
    variable_list_table = new QStandardItemModel();
    for(int i = 0; i < 32; i++) {
        debugger_variable[i] = new DEBUGGER_VARIABLE_T;
        variable_list_item[i] = new QStandardItem();
    }

    variable_list_table->setColumnCount(4);
    variable_list_table->setRowCount(1);
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
    ui->variable_list->setColumnWidth(2, 60);
}

void MainWindow::setup_custom_plot(void) {
    QPen pen;
    pen.setColor(Qt::blue);
    ui->custom_plot->legend->setVisible(true);
    ui->custom_plot->addGraph();
    ui->custom_plot->graph(0)->setPen(pen);
    ui->custom_plot->graph(0)->setName("Speed");

    pen.setColor(Qt::red);
    ui->custom_plot->addGraph();
    ui->custom_plot->graph(1)->setPen(pen);
    ui->custom_plot->graph(1)->setName("Angle");

    ui->custom_plot->xAxis->setLabel("t(s)");
    ui->custom_plot->yAxis->setLabel("y(rad/s rad)");

    ui->custom_plot->xAxis->setRange(0,1);
    ui->custom_plot->yAxis->setRange(0,1);

    connect(ui->custom_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->custom_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->yAxis2, SLOT(setRange(QCPRange)));

    ui->custom_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}