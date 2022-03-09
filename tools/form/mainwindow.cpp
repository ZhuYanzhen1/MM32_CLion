#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSimpleUpdater.h>

void MainWindow::downloadFinished(const QString &url, const QString &filepath) {
    (void) filepath;
    if (url == update_url) {
        QDir dir;
        if(!dir.exists(QCoreApplication::applicationDirPath() + "/extract")){
            dir.mkdir(QCoreApplication::applicationDirPath() + "/extract");
        }
        QFile::copy(QCoreApplication::applicationDirPath() + "/download/monitor.exe",
                    QCoreApplication::applicationDirPath() + "/extract/monitor.exe");
        QMessageBox::information(NULL, "Info", "Download finished! Restart the program to take effect.");
        QString updater_path = QCoreApplication::applicationDirPath() + "/updater.exe";
        QDesktopServices::openUrl(QUrl::fromLocalFile(updater_path));
        QCoreApplication::quit();
    }
}

void MainWindow::monitor_check_update(void) {
    updater = QSimpleUpdater::getInstance();
    connect(updater, SIGNAL(downloadFinished(QString, QString)), this, SLOT(downloadFinished(QString, QString)));
    updater->setModuleName(update_url, "monitor");
    updater->setModuleVersion(update_url, "1.0");
    updater->setNotifyOnFinish(update_url, false);
    updater->setNotifyOnUpdate(update_url, true);
    updater->setUseCustomAppcast(update_url, false);
    updater->setDownloaderEnabled(update_url, true);
    updater->setMandatoryUpdate(update_url, false);
    updater->checkForUpdates(update_url);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    QDir dir;
    dir.setPath(QCoreApplication::applicationDirPath() + "/download");
    dir.removeRecursively();
    dir.setPath(QCoreApplication::applicationDirPath() + "/extract");
    dir.removeRecursively();

    refresh_serial_port();
    setup_serial_wire("115200");
    setup_variable_table();
    setup_custom_plot();
    monitor_check_update();

    ui->compass_widget->set_direction(105.7f);
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
