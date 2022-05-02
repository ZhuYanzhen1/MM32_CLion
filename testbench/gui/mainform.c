//
// Created by LaoZhu on 2022/5/3.
//

#include "mainform.h"

void print_hello(GtkWidget *widget,
                 gpointer data) {
    g_print("Hello World\n");
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);

    button = gtk_button_new_with_label("Hello World");
    g_signal_connect (button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_window_set_child(GTK_WINDOW (window), button);

    gtk_window_present(GTK_WINDOW (window));
}
