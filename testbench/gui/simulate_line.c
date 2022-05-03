//
// Created by LaoZhu on 2022/5/3.
//

#include "simulate_line.h"
#include "test_data.h"

#define TRAJECTORY_ARRAY    test_point_1

void find_min_point(float *minx, float *miny) {
    *miny = TRAJECTORY_ARRAY[0][0];
    *minx = TRAJECTORY_ARRAY[0][1];
    for (int counter = 0; counter < (INDEX_NUM + 1); ++counter) {
        if (TRAJECTORY_ARRAY[counter][0] < *miny)
            *miny = TRAJECTORY_ARRAY[counter][0];
        if (TRAJECTORY_ARRAY[counter][1] < *minx)
            *minx = TRAJECTORY_ARRAY[counter][1];
    }
}

void find_max_point(float *minx, float *miny) {
    *miny = TRAJECTORY_ARRAY[0][0];
    *minx = TRAJECTORY_ARRAY[0][1];
    for (int counter = 0; counter < (INDEX_NUM + 1); ++counter) {
        if (TRAJECTORY_ARRAY[counter][0] > *miny)
            *miny = TRAJECTORY_ARRAY[counter][0];
        if (TRAJECTORY_ARRAY[counter][1] > *minx)
            *minx = TRAJECTORY_ARRAY[counter][1];
    }
}

void draw_track(cairo_t *cr, int width, int height) {
    int x_y_offset = 50;
    float min_north = 0, min_east = 0, max_north = 0, max_east = 0;
    double north_scale_factor, east_scale_factor, total_scale_factor;
    double *points_x = malloc((INDEX_NUM + 1) * 8);
    double *points_y = malloc((INDEX_NUM + 1) * 8);

    find_min_point(&min_east, &min_north);
    find_max_point(&max_east, &max_north);
    north_scale_factor = (double) (height - x_y_offset) / (double) (max_north - min_north);
    east_scale_factor = (double) (width - x_y_offset) / (double) (max_east - min_east);
    total_scale_factor = east_scale_factor < north_scale_factor ? east_scale_factor : north_scale_factor;

    for (int counter = 0; counter < (INDEX_NUM + 1); ++counter) {
        points_y[counter] = (TRAJECTORY_ARRAY[counter][0] - min_north) * total_scale_factor + (double) x_y_offset / 2;
        points_x[counter] = (TRAJECTORY_ARRAY[counter][1] - min_east) * total_scale_factor + (double) x_y_offset / 2;
    }

    cairo_move_to(cr, points_x[0], points_y[0]);
    for (int counter = 1; counter < (INDEX_NUM + 1); ++counter)
        cairo_line_to(cr, points_x[counter], points_y[counter]);

    free(points_x);
    free(points_y);
}
