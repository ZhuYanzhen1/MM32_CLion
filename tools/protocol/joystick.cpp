 #include "joystick.h"
#include <QMessageBox>
#include <QDebug>

static joyinfoex_tag     now_act;

QLibrary mylib("Winmm.dll");
MyPrototype qJoySetCapture = (MyPrototype)mylib.resolve("joySetCapture");
MyPrototype1 qJoyReleaseCapture = (MyPrototype1)mylib.resolve("joyReleaseCapture");
MyPrototype2 qJoyGetPosEx = (MyPrototype2)mylib.resolve("joyGetPosEx");
MyPrototype0 qJoyGetPos = (MyPrototype0)mylib.resolve("joyGetPos");
MyPrototype3 qJoyGetThreshold = (MyPrototype3)mylib.resolve("joyGetThreshold");
MyPrototype4 qJoyGetDevCaps = (MyPrototype4)mylib.resolve("joyGetDevCapsW");
MyPrototype5 qJoyGetNumDevs = (MyPrototype5)mylib.resolve("joyGetNumDevs");

void openJoy() {
    JOYCAPS joycaps;
    qJoyGetDevCaps(JSX, &joycaps, sizeof(joycaps));
}

struct joyinfoex_tag joyRead_row() {
    now_act.dwSize = sizeof(joyinfoex_tag);
    now_act.dwFlags = (int)JOY_RETURNBUTTONS;
    qJoyGetPosEx(JSX, &now_act);
    return now_act;
}
