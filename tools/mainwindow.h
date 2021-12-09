#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTextEdit>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_open_serial_btn_clicked();

public slots:
    void serial_received();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    /* serial configure functions */
    void refresh_serial_port();
    bool set_serial_badurate();

    /* mdtp related functions */
    void mdtp_data_transmit(unsigned char pid, const unsigned char *data);
    void mdtp_receive_handler(unsigned char data);
    void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
};

#endif // MAINWINDOW_H
