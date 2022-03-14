#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QLibrary>
#include <QString>
#include "windows.h"
#include "winuser.h"
#include "mmsystem.h"

#define  JSX             JOYSTICKID1 // ID

typedef int (*MyPrototype) (HWND, UINT, UINT, BOOL);
extern MyPrototype qJoySetCapture;

typedef void (*MyPrototype1) (UINT);
extern MyPrototype1 qJoyReleaseCapture ;

typedef void (*MyPrototype2) (UINT, LPJOYINFOEX);
extern MyPrototype2 qJoyGetPosEx;

typedef void (*MyPrototype0) (UINT, LPJOYINFO);
extern MyPrototype0 qJoyGetPos;

typedef void (*MyPrototype3) (UINT, LPUINT);
extern MyPrototype3 qJoyGetThreshold ;

typedef void (*MyPrototype4) (UINT, LPJOYCAPS, UINT);
extern MyPrototype4 qJoyGetDevCaps;

typedef UINT (*MyPrototype5) (void);
extern MyPrototype5 qJoyGetNumDevs;

void openJoy();
struct joyinfoex_tag joyRead_row();

#endif // JOYSTICK_H
