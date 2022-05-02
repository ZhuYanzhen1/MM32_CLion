//
// Created by LaoZhu on 2022/5/3.
//

#include "simulate_line.h"
#include "test_data.h"

void find_min_point(float *minx, float *miny) {
    *miny = test_point_1[0][0];
    *minx = test_point_1[0][1];
    for (int counter = 0; counter < sizeof(test_point_1) / (4 * sizeof(test_point_1[0][0])); ++counter) {
        if (test_point_1[counter][0] < *miny)
            *miny = test_point_1[counter][0];
        if (test_point_1[counter][1] < *minx)
            *minx = test_point_1[counter][1];
    }
}

void find_max_point(float *minx, float *miny) {
    *miny = test_point_1[0][0];
    *minx = test_point_1[0][1];
    for (int counter = 0; counter < sizeof(test_point_1) / (4 * sizeof(test_point_1[0][0])); ++counter) {
        if (test_point_1[counter][0] > *miny)
            *miny = test_point_1[counter][0];
        if (test_point_1[counter][1] > *minx)
            *minx = test_point_1[counter][1];
    }
}

void draw_track(cairo_t *cr, int width, int height) {
    int x_y_offset = 50;
    float min_north = 0, min_east = 0, max_north = 0, max_east = 0;
    double north_scale_factor, east_scale_factor, total_scale_factor;
    double *points_x = malloc(2 * sizeof(test_point_1) / sizeof(test_point_1[0][0]));
    double *points_y = malloc(2 * sizeof(test_point_1) / sizeof(test_point_1[0][0]));

    find_min_point(&min_east, &min_north);
    find_max_point(&max_east, &max_north);
    north_scale_factor = (double) (height - x_y_offset) / (double) (max_north - min_north);
    east_scale_factor = (double) (width - x_y_offset) / (double) (max_east - min_east);
    total_scale_factor = east_scale_factor < north_scale_factor ? east_scale_factor : north_scale_factor;

    for (int counter = 0; counter < sizeof(test_point_1) / (4 * sizeof(test_point_1[0][0])); ++counter) {
        points_y[counter] = (test_point_1[counter][0] - min_north) * total_scale_factor + (double) x_y_offset / 2;
        points_x[counter] = (test_point_1[counter][1] - min_east) * total_scale_factor + (double) x_y_offset / 2;
    }

    cairo_move_to(cr, points_x[0], points_y[0]);
    for (int counter = 1; counter < sizeof(test_point_1) / (4 * sizeof(test_point_1[0][0])); ++counter)
        cairo_line_to(cr, points_x[counter], points_y[counter]);

    free(points_x);
    free(points_y);
}
