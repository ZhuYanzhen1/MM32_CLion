#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTextEdit>
#include <QSlider>
#include <QStandardItemModel>
#include "joy_thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSimpleUpdater;

typedef struct DEBUGGER_VARIABLE_T {
    unsigned int var_status;
    unsigned int var_address;
    unsigned int var_value;
    char var_name[16];
    QStandardItem *value_item;
    QVector<double> *x;
    QVector<double> *y;
    unsigned int curve_point_counter;
} debugger_variable_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_open_serial_btn_clicked();
    void on_refresh_serial_btn_clicked();
    void on_variable_list_clicked(const QModelIndex &index);
    void on_clear_btn_clicked();
    void downloadFinished(const QString &url, const QString &filepath);

public slots:
    void serial_received();
    void display_slot_row(joyinfoex_tag state_row);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QSimpleUpdater *updater;
    Joy_Thread *joy_thread;
    const QString update_url = "http://www.acmetech.top/wp-content/monitor_update/updates.json";

    QStandardItemModel *variable_list_table;
    QStandardItem *variable_list_item[32];
    debugger_variable_t *debugger_variable[32];
    unsigned char plot_curve_counter = 0;
    unsigned char plot_curve_list[12] = {0};
    unsigned char checked_list_last[12] = {0};

    const QColor back_color_table[12] = {QColor(0xff, 0x00, 0x00), QColor(0x00, 0xff, 0x00), QColor(0x00, 0x00, 0xff), QColor(0x85, 0x52, 0xa1),
                                           QColor(0x65, 0xc2, 0x94), QColor(0x00, 0xa6, 0xac), QColor(0xf3, 0x6c, 0x21), QColor(0x00, 0x53, 0x44),
                                           QColor(0xea, 0x66, 0xa6), QColor(0x98, 0x71, 0x65), QColor(0xb2, 0xd2, 0x35), QColor(0xff, 0xd4, 0x00)};
    const QColor font_color_table[12] = {QColor(0x00, 0x00, 0x00), QColor(0x00, 0x00, 0x00), QColor(0xf0, 0xf0, 0xf0), QColor(0xf0, 0xf0, 0xf0),
                                           QColor(0x00, 0x00, 0x00), QColor(0x00, 0x00, 0x00), QColor(0x00, 0x00, 0x00), QColor(0xf0, 0xf0, 0xf0),
                                           QColor(0x00, 0x00, 0x00), QColor(0xf0, 0xf0, 0xf0), QColor(0x00, 0x00, 0x00), QColor(0x00, 0x00, 0x00)};

    void monitor_check_update(void);

    /* serial configure functions */
    void refresh_serial_port();
    bool set_serial_badurate();
    void clear_debugger_variable(void);

    /* mdtp related functions */
    void mdtp_data_transmit(unsigned char pid, const unsigned char *data);
    void mdtp_receive_handler(unsigned char data);
    void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
    void setup_serial_wire(QString baudrate);

    /* variable list and plot related functions */
    void table_setvalue_variable(unsigned char index, unsigned int value, unsigned int timestamp);
    void table_append_variable(unsigned char type, unsigned int value, const char *name, unsigned int address);
    void setup_variable_table(void);
    void setup_custom_plot(void);
    void clear_table_variable(void);
};

#endif // MAINWINDOW_H
