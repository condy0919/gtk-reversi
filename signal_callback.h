#ifndef SIGNAL_CALLBACK_H
#define SIGNAL_CALLBACK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include "commondefs.h"
#include "gui.h"
#include "interface.h"
#include "engine.h"
#include "list.h"

/*
 * TODO: other OS
 */
#ifdef linux
#include <unistd.h>
#endif

// app windwo's exit function
void on_window_destroy(GtkObject* object, gtk_app_t* gtk_app);


// app window start function
void on_start_button_click(GtkButton* button, gtk_app_t* gtk_app);



int on_configure_event(GtkWidget* draw_area, GdkEventConfigure* event, gtk_app_t* gtk_app);
int on_expose_event(GtkWidget* widget, GdkEventExpose* event, gtk_app_t* gtk_app);
int on_button_press_event(GtkWidget* draw_area, GdkEventButton* event, gtk_app_t* gtk_app);

void show_pixmap(gtk_app_t* gtk_app);

#endif
