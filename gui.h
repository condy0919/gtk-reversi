#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "commondefs.h"
#include "signal_callback.h"
#include "interface.h"


/** 
 * init the gtk reversi app window, and register some signal handlers.
 * 
 * @param gtk_app 
 */
void init_gui(gtk_app_t* gtk_app);

GdkPixbuf* create_pixbuf(const gchar* filename);

void game_is_over(gtk_app_t* gtk_app);

#endif
