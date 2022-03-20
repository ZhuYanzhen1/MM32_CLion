//
// Created by Lao·Zhu on 2021/4/29.
//

#ifndef PROGRAM_GUI_GUI_BUTTON_H_
#define PROGRAM_GUI_GUI_BUTTON_H_

#define BUTTON_COLOR_NORMAL 0xE73C
#define BUTTON_COLOR_NORMAL_EDGE 0xCE59

#define BUTTON_COLOR_CLICK 0xF719
#define BUTTON_COLOR_CLICK_EDGE 0x9AA0

typedef enum {
    Button_Normal = 0,
    Button_Click = 1,
} Button_State_e;

typedef struct BUTTON_TYPE_T {
    unsigned char x_pos;
    unsigned char y_pos;
    unsigned char width;
    unsigned char height;
    const char *Text;
    void (*CallbackFunction)(void *Object, unsigned char Key);
    Button_State_e Status;
} Button_Struct_t;

void gui_button_init(Button_Struct_t *button);
void gui_button_update(Button_Struct_t *button, Button_State_e status);

#endif //PROGRAM_GUI_GUI_BUTTON_H_
