//
// Created by LaoZhu on 2022/5/3.
//

#include "simulate_line.h"
#include "test_data.h"

#define TRAJECTORY_ARRAY    test_point_1
#define ACTUAL_TRAJECTORY   pos

extern float pos[100000][2];
extern unsigned int max_pos;

static double total_scale_factor = 0;
static int x_offset = 50, y_offset = 50;

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
    float min_north = 0, min_east = 0, max_north = 0, max_east = 0;
    double north_scale_factor, east_scale_factor;
    double *points_x = malloc((INDEX_NUM + 1) * 8);
    double *points_y = malloc((INDEX_NUM + 1) * 8);

    find_min_point(&min_east, &min_north);
    find_max_point(&max_east, &max_north);
    north_scale_factor = (double) (height - y_offset) / (double) (max_north - min_north);
    east_scale_factor = (double) (width - x_offset) / (double) (max_east - min_east);
    total_scale_factor = east_scale_factor < north_scale_factor ? east_scale_factor : north_scale_factor;
    x_offset = (int) ((double) width - ((double) (max_east - min_east) * total_scale_factor)) / 2;
    y_offset = (int) ((double) height - ((double) (max_north - min_north) * total_scale_factor)) / 2;

    for (int counter = 0; counter < (INDEX_NUM + 1); ++counter) {
        points_x[counter] = (TRAJECTORY_ARRAY[counter][1] - min_east) * total_scale_factor + x_offset;
        points_y[counter] = (TRAJECTORY_ARRAY[counter][0] - min_north) * total_scale_factor + y_offset;
    }

    cairo_move_to(cr, points_x[0], points_y[0]);
    for (int counter = 1; counter < (INDEX_NUM + 1); ++counter)
        cairo_line_to(cr, points_x[counter], points_y[counter]);

    free(points_x);
    free(points_y);
}

void draw_actual_trace(cairo_t *cr) {
    float min_north = 0, min_east = 0;
    double *points_x = malloc(8 * max_pos);
    double *points_y = malloc(8 * max_pos);
    find_min_point(&min_east, &min_north);

    for (int counter = 0; counter < max_pos; ++counter) {
        points_x[counter] = (ACTUAL_TRAJECTORY[counter][1] - min_east) * total_scale_factor + x_offset;
        points_y[counter] = (ACTUAL_TRAJECTORY[counter][0] - min_north) * total_scale_factor + y_offset;
    }

    cairo_move_to(cr, points_x[0], points_y[0]);
    for (int counter = 1; counter < max_pos; ++counter)
        cairo_line_to(cr, points_x[counter], points_y[counter]);

    free(points_x);
    free(points_y);
}
