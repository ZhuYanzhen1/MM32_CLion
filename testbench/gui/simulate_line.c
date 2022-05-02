//
// Created by LaoZhu on 2022/5/3.
//

#include "simulate_line.h"
#include "test_data.h"

void draw_track(cairo_t *cr, int width, int height) {
    float min_north, min_east;
    for (int counter = 0; counter < sizeof(); ++counter) {

    }
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 100 + counter * 6, (double) height / 2 + sin((double) counter / 8.0) * 100);
}
