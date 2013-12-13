#include "gui.h"

/**
 * @file   gtk_main.c
 * @author condy chen <condy0919@gmail.com>
 * @date   Fri Dec 13 01:12:00 2013
 * 
 * @brief  Gtk based reversi game.
 * 
 * 
 */


int main(int argc, char* argv[])
{
	gtk_app_t* gtk_app = g_slice_new(gtk_app_t);

	// initialize GTK
	gtk_set_locale();
	gtk_init(&argc, &argv);

	init_gui(gtk_app);
	gtk_main();

	g_slice_free(gtk_app_t, gtk_app);

	return 0;
}
