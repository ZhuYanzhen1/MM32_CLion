#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTextEdit>
#include <QSlider>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct DEBUGGER_VARIABLE_T {
    unsigned int var_status;
    unsigned int var_address;
    unsigned int var_value;
    char var_name[16];
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

public slots:
    void serial_received();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    QStandardItemModel *variable_list_table;
    QStandardItem *variable_list_item[32];
    debugger_variable_t *debugger_variable[32];

    /* serial configure functions */
    void refresh_serial_port();
    bool set_serial_badurate();

    /* mdtp related functions */
    void mdtp_data_transmit(unsigned char pid, const unsigned char *data);
    void mdtp_receive_handler(unsigned char data);
    void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
    void setup_serial_wire(QString baudrate);

    /* variable list and plot related functions */
    void table_setvalue_variable(unsigned char index, unsigned int value);
    void table_append_variable(unsigned char type, unsigned int value, char *name, unsigned int address);
    void setup_variable_table(void);
    void setup_custom_plot(void);
};

#endif // MAINWINDOW_H
