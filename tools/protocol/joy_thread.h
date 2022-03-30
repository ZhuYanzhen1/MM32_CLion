#ifndef JOY_THREAD_H
#define JOY_THREAD_H

#include <QThread>
#include "joystick.h"
#include <string>
#include <iostream>
#include <fstream>

#define JOY_READ_PEROID  100

struct Joykey_Info {
    bool  A_botton=0;
    bool  B_botton=0;
    bool  X_botton=0;
    bool  Y_botton=0;
    bool  LB_botton=0;
    bool  RB_botton=0;
    bool  BACK_botton=0;
    bool  START_botton=0;
    bool  UP_botton=0;
    bool  DOWN_botton=0;
    bool  LEFT_botton=0;
    bool  RIGHT_botton=0;
    int   X_Axis=0;
    int   Y_Axis=0;
    int   U_Axis=0;
    int   R_Axis=0;
    int   Z_Axis=0;
};


class Joy_Thread : public QThread
{
    Q_OBJECT
public:
    struct Joykey_Info joykey_info; //test
    joyinfoex_tag state_row;
    std::ofstream file;
    explicit Joy_Thread(QObject *parent = 0);
    void copy_value_to_joykey_info();

signals:
    void JoySignal_row(joyinfoex_tag); //test

protected:
    void run();
    void send_state_row(joyinfoex_tag state_row);

};

#endif // JOY_THREAD_H
