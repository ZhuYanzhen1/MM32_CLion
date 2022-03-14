#include "joy_thread.h"
#include <QDebug>

Joy_Thread::Joy_Thread(QObject *parent) : QThread(parent) { }

void Joy_Thread::send_state_row(joyinfoex_tag state_row) {
    emit JoySignal_row(state_row);
}

void Joy_Thread::run() {
    openJoy();
    while(1) {
        msleep(JOY_READ_PEROID);
        state_row = joyRead_row();
        copy_value_to_joykey_info();
        send_state_row(state_row);
    }
}

void Joy_Thread::copy_value_to_joykey_info() {
    joykey_info.X_Axis=(state_row.dwXpos);
    joykey_info.Y_Axis=(state_row.dwYpos);
    joykey_info.U_Axis=(state_row.dwUpos);
    joykey_info.R_Axis=(state_row.dwRpos);
    joykey_info.Z_Axis=(state_row.dwZpos);
    if(state_row.dwButtons & 0x01 << 0)
        joykey_info.A_botton = 1;
    else
        joykey_info.A_botton = 0;

    if(state_row.dwButtons & 0x01 << 1)
        joykey_info.B_botton = 1;
    else
        joykey_info.B_botton = 0;

    if(state_row.dwButtons & 0x01 << 2)
        joykey_info.X_botton = 1;
    else
        joykey_info.X_botton = 0;

    if(state_row.dwButtons & 0x01 << 3)
        joykey_info.Y_botton = 1;
    else
        joykey_info.Y_botton = 0;

    if(state_row.dwButtons & 0x01 << 4)
        joykey_info.LB_botton = 1;
    else
        joykey_info.LB_botton = 0;

    if(state_row.dwButtons & 0x01 << 6)
        joykey_info.BACK_botton = 1;
    else
        joykey_info.BACK_botton = 0;

    if(state_row.dwButtons & 0x01 << 7)
        joykey_info.START_botton = 1;
    else
        joykey_info.START_botton = 0;

    if(state_row.dwButtons & 0x01 << 5)
        joykey_info.RB_botton = 1;
    else
        joykey_info.RB_botton = 0;

    if(state_row.dwPOV == 0)
        joykey_info.UP_botton = 1;
    else if(state_row.dwPOV == 9000)
        joykey_info.RIGHT_botton = 1;
    else if(state_row.dwPOV == 18000)
        joykey_info.DOWN_botton = 1;
    else if(state_row.dwPOV == 27000)
        joykey_info.LEFT_botton = 1;
    else if(state_row.dwPOV == 4500) {
        joykey_info.UP_botton = 1;
        joykey_info.RIGHT_botton = 1;
    } else if(state_row.dwPOV == 31500) {
        joykey_info.UP_botton = 1;
        joykey_info.LEFT_botton = 1;
    } else if(state_row.dwPOV == 13500) {
        joykey_info.RIGHT_botton = 1;
        joykey_info.DOWN_botton = 1;
    } else if(state_row.dwPOV == 22500) {
        joykey_info.LEFT_botton = 1;
        joykey_info.DOWN_botton = 1;
    } else {
        joykey_info.LEFT_botton = 0;
        joykey_info.DOWN_botton = 0;
        joykey_info.UP_botton = 0;
        joykey_info.RIGHT_botton = 0;
    }
}
