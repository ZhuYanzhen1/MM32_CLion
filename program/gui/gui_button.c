//
// Created by Lao·Zhu on 2021/4/29.
//

#include "gui_button.h"
#include "gui_base.h"
#include "string.h"

void gui_button_init(button_struct_t *button) {
    button->next_button = NULL;
}

void gui_button_settext(button_struct_t *button, const char *text) {
    button->text = text;
    unsigned char text_x = button->x_pos + (button->width - strlen(button->text) * 6) / 2;
    unsigned char text_y = button->y_pos + (button->height - 12) / 2;
    gui_draw_rectangle(button->x_pos + 1, text_y, button->width - 2, 12,
                       BUTTON_COLOR_NORMAL, Filled);
    gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", button->text);
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

void gui_button_scanbtn(unsigned char xpos, unsigned char ypos) {

}
