/**************************************************************************/ /**
    \file     gui_button.c
    \brief    gui button features function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#include "gui_button.h"
#include "gui_base.h"
#include "string.h"

void gui_button_init(button_struct_t *button,
                     unsigned char x_pos,
                     unsigned char y_pos,
                     unsigned char width,
                     unsigned char height,
                     const char *name) {
    button->next_button = NULL;
    button->status = button_normal_status;
    button->update_flag = 0;
    button->x_pos = x_pos;
    button->y_pos = y_pos;
    button->width = width;
    button->height = height;
    button->text = name;
}

void gui_button_settext(button_struct_t *button, const char *text) {
    button->text = text;
    button->update_flag = 1;
}

void gui_button_update(button_struct_t *button, button_state_e status) {
    unsigned char text_x = button->x_pos + (button->width - strlen(button->text) * 6) / 2;
    unsigned char text_y = button->y_pos + (button->height - 12) / 2;
    switch (status) {
        case button_click_status:
            gui_draw_rectangle(button->x_pos,
                               button->y_pos,
                               button->width,
                               button->height,
                               BUTTON_COLOR_CLICK,
                               Filled);
            gui_draw_rectangle(button->x_pos,
                               button->y_pos,
                               button->width,
                               button->height,
                               BUTTON_COLOR_CLICK_EDGE,
                               UnFilled);
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_CLICK, "%s", button->text);
            break;
        case button_normal_status:
            gui_draw_rectangle(button->x_pos,
                               button->y_pos,
                               button->width,
                               button->height,
                               BUTTON_COLOR_NORMAL,
                               Filled);
            gui_draw_rectangle(button->x_pos,
                               button->y_pos,
                               button->width,
                               button->height,
                               BUTTON_COLOR_NORMAL_EDGE,
                               UnFilled);
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", button->text);
            break;
        default:break;
    }
}
