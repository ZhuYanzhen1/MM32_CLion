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

void MainWindow::clear_table_variable(void) {
    variable_list_table->clear();
    variable_list_table->setColumnCount(4);
    variable_list_table->setRowCount(0);
    variable_list_table->setHeaderData(0,Qt::Horizontal,"name");
    variable_list_table->setHeaderData(1,Qt::Horizontal,"value");
    variable_list_table->setHeaderData(2,Qt::Horizontal,"type");
    variable_list_table->setHeaderData(3,Qt::Horizontal,"address");
    for(int i = 0; i < 32; i++) {
        variable_list_item[i]->setCheckable(true);
        variable_list_item[i]->setCheckState(Qt::Unchecked);
    }
    ui->variable_list->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->variable_list->setModel(variable_list_table);
    ui->variable_list->setColumnWidth(0, 110);
    ui->variable_list->setColumnWidth(1, 110);
    ui->variable_list->setColumnWidth(2, 60);
    ui->variable_list->setColumnWidth(3, 110);
    variable_counter = 0;
}

void MainWindow::table_setvalue_variable(unsigned char index, unsigned int value, unsigned int timestamp) {
    unsigned char *ptr_u8 = (unsigned char *)&value;
    if(index < variable_counter) {
        double data_value = 0;
        switch (debugger_variable[index]->var_status) {
            case signed_int8:
                data_value = (double)(*(char *)&ptr_u8[0]);
                debugger_variable[index]->value_item->setText(QString::number(*(char *)&ptr_u8[0]));
                break;
            case signed_int16:
                data_value = (double)(*(short *)&ptr_u8[0]);
                debugger_variable[index]->value_item->setText(QString::number(*(short *)&ptr_u8[0]));
                break;
            case signed_int32:
                data_value = (double)(*(int *)&value);
                debugger_variable[index]->value_item->setText(QString::number(*(int *)&value));
                break;
            case unsigned_int8:
                data_value = (double)(*(unsigned char *)&ptr_u8[0]);
                debugger_variable[index]->value_item->setText(QString::number(*(unsigned char *)&ptr_u8[0]));
                break;
            case unsigned_int16:
                data_value = (double)(*(unsigned short *)&ptr_u8[0]);
                debugger_variable[index]->value_item->setText(QString::number(*(unsigned short *)&ptr_u8[0]));
                break;
            case unsigned_int32:
                data_value = (double)(*(unsigned int *)&value);
                debugger_variable[index]->value_item->setText(QString::number(*(unsigned int *)&value));
                break;
            case float_32bit:
                data_value = (double)(*(float *)&value);
                debugger_variable[index]->value_item->setText(QString::number((*(float *)&value), 'f', 3));
                break;
        }
        debugger_variable[index]->curve_point_counter = debugger_variable[index]->curve_point_counter + 1;
        debugger_variable[index]->x->resize(debugger_variable[index]->curve_point_counter);
        debugger_variable[index]->y->resize(debugger_variable[index]->curve_point_counter);
        debugger_variable[index]->x->append(((double)timestamp / 1000.0));
        debugger_variable[index]->y->append(data_value);

        int plot_curve_index = -1;
        for(int counter = 0; counter < 12; counter++)
            if(plot_curve_list[counter] == (index + 1))
                plot_curve_index = counter;

        if(plot_curve_index != -1) {
            ui->custom_plot->graph(plot_curve_index)->setData(*(debugger_variable[index]->x), *(debugger_variable[index]->y));
            ui->custom_plot->graph(plot_curve_index)->rescaleAxes();
        }
        ui->custom_plot->replot();

        if(QString(debugger_variable[index]->var_name) == "compass")
            ui->compass_widget->set_direction(data_value);
    }
}

void MainWindow::table_append_variable(unsigned char type, unsigned int value, const char *name, unsigned int address) {
    unsigned char *ptr_u8 = (unsigned char *)&value;

    debugger_variable[variable_counter]->x->clear();
    debugger_variable[variable_counter]->y->clear();
    debugger_variable[variable_counter]->var_status = type;
    debugger_variable[variable_counter]->var_address = address;
    debugger_variable[variable_counter]->var_value = value;
    for(unsigned char counter = 0; counter < 14; counter++)
        debugger_variable[variable_counter]->var_name[counter] = name[counter];

    switch (debugger_variable[variable_counter]->var_status) {
        case signed_int8:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number(*(char *)&ptr_u8[0]));
            break;
        case signed_int16:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number(*(short *)&ptr_u8[0]));
            break;
        case signed_int32:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number(*(int *)&value));
            break;
        case unsigned_int8:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number(*(unsigned char *)&ptr_u8[0]));
            break;
        case unsigned_int16:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number(*(unsigned short *)&ptr_u8[0]));
            break;
        case unsigned_int32:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number(*(unsigned int *)&value));
            break;
        case float_32bit:
            debugger_variable[variable_counter]->value_item = new QStandardItem(QString::number((*(float *)&value), 'f', 3));
            break;
    }

    variable_list_table->setRowCount(variable_counter + 1);
    variable_list_table->setItem(variable_counter, 0, variable_list_item[variable_counter]);
    switch(type) {
        case signed_int8:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("int8"));
        break;
        case signed_int16:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("int16"));
        break;
        case signed_int32:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("int32"));
        break;
        case unsigned_int8:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("uint8"));
        break;
        case unsigned_int16:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("uint16"));
        break;
        case unsigned_int32:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("uint32"));
        break;
        case float_32bit:
            variable_list_table->setItem(variable_counter, 2, new QStandardItem("float"));
        break;
    }
    table_setvalue_variable(variable_counter, value, 0);
    variable_list_table->setItem(variable_counter, 1, debugger_variable[variable_counter]->value_item);
    variable_list_table->setItem(variable_counter, 3, new QStandardItem("0x" + QString::number(address, 16)));
    variable_list_item[variable_counter]->setBackground(back_color_table[variable_counter]);
    variable_list_item[variable_counter]->setForeground(font_color_table[variable_counter]);
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
        debugger_variable[i]->x = new QVector<double>(1);
        debugger_variable[i]->y = new QVector<double>(1);
        debugger_variable[i]->curve_point_counter = 1;
    }

    variable_list_table->setColumnCount(4);
    variable_list_table->setRowCount(0);
    variable_list_table->setHeaderData(0,Qt::Horizontal,"name");
    variable_list_table->setHeaderData(1,Qt::Horizontal,"value");
    variable_list_table->setHeaderData(2,Qt::Horizontal,"type");
    variable_list_table->setHeaderData(3,Qt::Horizontal,"address");
    for(int i = 0; i < 32; i++) {
        variable_list_item[i]->setCheckable(true);
        variable_list_item[i]->setCheckState(Qt::Unchecked);
    }
    ui->variable_list->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->variable_list->setModel(variable_list_table);
    ui->variable_list->setColumnWidth(0, 110);
    ui->variable_list->setColumnWidth(1, 110);
    ui->variable_list->setColumnWidth(2, 60);
    ui->variable_list->setColumnWidth(3, 110);
}

void MainWindow::setup_custom_plot(void) {
    ui->custom_plot->legend->setVisible(true);
    ui->custom_plot->xAxis->setLabel("t(s)");
    ui->custom_plot->yAxis->setLabel("y(value)");
    ui->custom_plot->xAxis->setRange(-1, 1);
    ui->custom_plot->yAxis->setRange(-1, 1);
    connect(ui->custom_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->custom_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->yAxis2, SLOT(setRange(QCPRange)));
    ui->custom_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->map_plot->legend->setVisible(true);
    ui->map_plot->xAxis->setLabel("x(m)");
    ui->map_plot->yAxis->setLabel("y(m)");
    ui->map_plot->xAxis->setRange(-100, 100);
    ui->map_plot->yAxis->setRange(-100, 100);
    connect(ui->map_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->map_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->yAxis2, SLOT(setRange(QCPRange)));
    ui->map_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidthF(3);
    ui->map_plot->addGraph();
    ui->map_plot->graph(0)->setPen(pen);
    ui->map_plot->graph(0)->setName("Track");
    ui->map_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->map_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    ui->map_plot->replot();
}
