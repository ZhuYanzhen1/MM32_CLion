//
// Created by Lao·Zhu on 2021/4/29.
//

#include "gui_button.h"
#include "gui_base.h"
#include "string.h"

void gui_button_init(Button_Struct_t *button) {
    unsigned char text_x = button->x_pos + (button->width - strlen(button->Text) * 6) / 2;
    unsigned char text_y = button->y_pos + (button->height - 12) / 2;
    button->Status = Button_Normal;
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
    gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", button->Text);
}

void gui_button_update(Button_Struct_t *button, Button_State_e status) {
    unsigned char text_x = button->x_pos + (button->width - strlen(button->Text) * 6) / 2;
    unsigned char text_y = button->y_pos + (button->height - 12) / 2;
    button->Status = status;
    switch (status) {
        case Button_Click:
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
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_CLICK, "%s", button->Text);
            break;
        case Button_Normal:
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
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", button->Text);
            break;
        default:break;
    }
}