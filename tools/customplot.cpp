#include "mainwindow.h"
#include "./ui_mainwindow.h"

static const unsigned char signed_int8 = 0;
static const unsigned char signed_int16 = 1;
static const unsigned char signed_int32 = 2;
static const unsigned char unsigned_int8 = 3;
static const unsigned char unsigned_int16 = 4;
static const unsigned char unsigned_int32 = 5;
static const unsigned char float_32bit = 6;
static unsigned char variable_counter = 0;

static const QColor color_table[12] = {QColor(0xff, 0x00, 0x00), QColor(0x00, 0xff, 0x00), QColor(0x00, 0x00, 0xff), QColor(0x85, 0x52, 0xa1),
                                       QColor(0x65, 0xc2, 0x94), QColor(0x00, 0xa6, 0xac), QColor(0xf3, 0x6c, 0x21), QColor(0x00, 0x53, 0x44),
                                       QColor(0xea, 0x66, 0xa6), QColor(0x98, 0x71, 0x65), QColor(0xb2, 0xd2, 0x35), QColor(0xff, 0xd4, 0x00)};

void MainWindow::table_setvalue_variable(unsigned char index, unsigned int value) {
    unsigned char *ptr_u8 = (unsigned char *)&value;
//    qDebug() << "0x" + QString::number(ptr_u8[0], 16) << "  0x" + QString::number(ptr_u8[1], 16)
//            << "  0x" + QString::number(ptr_u8[2], 16) << "  0x" + QString::number(ptr_u8[3], 16);
    switch (debugger_variable[index]->var_status) {
        case signed_int8:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number(*(char *)&ptr_u8[0])));
            break;
        case signed_int16:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number(*(short *)&ptr_u8[0])));
            break;
        case signed_int32:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number(*(int *)&value)));
            break;
        case unsigned_int8:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number(*(unsigned char *)&ptr_u8[0])));
            break;
        case unsigned_int16:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number(*(unsigned short *)&ptr_u8[0])));
            break;
        case unsigned_int32:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number(*(unsigned int *)&value)));
            break;
        case float_32bit:
            variable_list_table->setItem(index, 2, new QStandardItem(QString::number((*(float *)&value), 'f', 3)));
            break;
    }
}

void MainWindow::table_append_variable(unsigned char type, unsigned int value, const char *name, unsigned int address) {
    debugger_variable[variable_counter]->var_status = type;
    debugger_variable[variable_counter]->var_address = address;
    debugger_variable[variable_counter]->var_value = value;
    for(unsigned char counter = 0; counter < 14; counter++)
        debugger_variable[variable_counter]->var_name[counter] = name[counter];
    variable_list_table->setRowCount(variable_counter + 1);
    variable_list_table->setItem(variable_counter, variable_list_item[variable_counter]);
    switch(type) {
        case signed_int8:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("int8"));
        break;
        case signed_int16:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("int16"));
        break;
        case signed_int32:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("int32"));
        break;
        case unsigned_int8:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("uint8"));
        break;
        case unsigned_int16:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("uint16"));
        break;
        case unsigned_int32:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("uint32"));
        break;
        case float_32bit:
            variable_list_table->setItem(variable_counter, 1, new QStandardItem("float"));
        break;
    }
    table_setvalue_variable(variable_counter, value);
    variable_list_table->setItem(variable_counter, 3, new QStandardItem("0x" + QString::number(address, 16)));
    variable_list_item[variable_counter]->setBackground(color_table[variable_counter]);
    variable_list_item[variable_counter]->setText(name);
    variable_counter++;
}

void MainWindow::setup_variable_table(void) {
    variable_list_table = new QStandardItemModel();
    for(int i = 0; i < 32; i++) {
        debugger_variable[i] = new DEBUGGER_VARIABLE_T;
        variable_list_item[i] = new QStandardItem();
        debugger_variable[i]->var_name[15] = 0xff;
        debugger_variable[i]->var_name[14] = 0x00;
    }

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

void MainWindow::setup_custom_plot(void) {
    ui->custom_plot->legend->setVisible(true);
    QPen pen;

//    pen.setColor(color_table[0]);
//    ui->custom_plot->addGraph();
//    ui->custom_plot->graph(0)->setPen(pen);
//    ui->custom_plot->graph(0)->setName("Speed");

    ui->custom_plot->xAxis->setLabel("t(s)");
    ui->custom_plot->yAxis->setLabel("y(value)");

    ui->custom_plot->xAxis->setRange(0, 10);
    ui->custom_plot->yAxis->setRange(-10, 10);

    connect(ui->custom_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->custom_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->yAxis2, SLOT(setRange(QCPRange)));

    ui->custom_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
