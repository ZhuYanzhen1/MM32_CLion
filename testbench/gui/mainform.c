//
// Created by LaoZhu on 2022/5/3.
//

#include "mainform.h"
#include "gtk/gtk.h"
#include "simulate_line.h"
#include "simulate.h"

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
//    double dashes[] = {2.0,  /* ink */
//                       2.0,  /* skip */
//                       2.0,  /* ink */
//                       2.0   /* skip */
//    };
//    cairo_set_dash(cr, dashes, sizeof(dashes) / sizeof(dashes[0]), -50.0);
    cairo_set_line_width(cr, 2.0);
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_move_to(cr, 0, 0);
    draw_track(cr, width, height);
    simulate_lqr_control();
    draw_actual_trace(cr);
    cairo_stroke(cr);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *win = gtk_application_window_new(GTK_APPLICATION (app));
    GtkWidget *area = gtk_drawing_area_new();

    gtk_window_set_default_size(GTK_WINDOW(win), 800, 500);
    gtk_window_set_title(GTK_WINDOW (win), "Mainform");
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (area), draw_function, NULL, NULL);
    gtk_window_set_child(GTK_WINDOW (win), area);

    gtk_widget_show(win);
}

int mainform_config(int argc, char **argv) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.freescale.unittest", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return status;
}