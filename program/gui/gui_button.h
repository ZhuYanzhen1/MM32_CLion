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
    button_normal_status = 0,
    button_click_status = 1,
} button_state_e;

typedef struct BUTTON_TYPE_T {
    unsigned char x_pos;
    unsigned char y_pos;
    unsigned char width;
    unsigned char height;
    const char *Text;
    void (*CallbackFunction)(void *Object, unsigned char Key);
    button_state_e status;
} button_struct_t;

void gui_button_init(button_struct_t *button);
void gui_button_update(button_struct_t *button, button_state_e status);

#endif //PROGRAM_GUI_GUI_BUTTON_H_
