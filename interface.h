#ifndef INTERFACE_H
#define INTERFACE_H

#include "commondefs.h"
#include "list.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

/*
 * modify some GUI elements through this file.
 */

void update_both_scores(gtk_app_t* gtk_app, int player, int computer);

void set_color(gtk_app_t* gtk_app, int x, int y, int color);

void turn_around_pieces(gtk_app_t* gtk_app, int x, int y, int color);

#endif
