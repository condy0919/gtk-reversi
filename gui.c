#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"

/** 
 * initialize the gtk app's gui also some signals.
 * 
 * @param gtk_app 
 */
void init_gui(gtk_app_t* gtk_app)
{
	gtk_app->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// set window title
	gtk_window_set_title(GTK_WINDOW(gtk_app->window), "Gtk-Reversi");

	// set window default size
	gtk_window_set_default_size(GTK_WINDOW(gtk_app->window),
			BOARD_WIDTH + 2 * XOFFSET + 2 * XBORDER, 
			BOARD_HEIGHT + 2 * YOFFSET + 2 * YBORDER);

	// set window border width
	gtk_container_set_border_width(GTK_CONTAINER(gtk_app->window), 0);

	// keep window un-resizable
	gtk_window_set_resizable(GTK_WINDOW(gtk_app->window), FALSE);

	// bind destory signal
	g_signal_connect(gtk_app->window, "destroy",
			G_CALLBACK(on_window_destroy), gtk_app);


	// add the vbox to the window and show vbox
	gtk_app->vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_app->window), gtk_app->vbox);


	// allocate a new drawing area to contain chips
	gtk_app->draw_area = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(gtk_app->vbox),
			gtk_app->draw_area, TRUE, TRUE, 0);


	// set draw area size
	gtk_widget_set_size_request(GTK_WIDGET(gtk_app->draw_area),
			BOARD_WIDTH + 2 * XOFFSET + 2 * XBORDER,
			BOARD_HEIGHT + 2 * YOFFSET + 2 * YBORDER);


	// bind signals with specified callback functions
	// when player click in chips
	g_signal_connect(G_OBJECT(gtk_app->draw_area), "button_press_event",
			G_CALLBACK(on_button_press_event), gtk_app);

	// when the prog will be configured
	g_signal_connect(G_OBJECT(gtk_app->draw_area), "configure_event",
			G_CALLBACK(on_configure_event), gtk_app);

	// redraw the contents of the widget
	g_signal_connect(G_OBJECT(gtk_app->draw_area), "expose_event",
			G_CALLBACK(on_expose_event), gtk_app);

	// register the events
	gtk_widget_set_events(gtk_app->draw_area,
			GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);


	gtk_app->hbox = gtk_hbox_new(FALSE, 0);
	gtk_app->player_score = gtk_label_new("");
	gtk_app->computer_score = gtk_label_new("");
	// append labels and start button to HBox
	gtk_app->start_button = gtk_button_new_with_label("start");

	// bind start signal with `start button'
	g_signal_connect(G_OBJECT(gtk_app->start_button), "clicked",
			G_CALLBACK(on_start_button_click), gtk_app);
	gtk_box_pack_start(GTK_BOX(gtk_app->hbox), gtk_app->player_score,
			TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_app->hbox), gtk_app->computer_score,
			TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_app->hbox), gtk_app->start_button,
			TRUE, TRUE, 0);


	// by the way, update both labels with 0
	update_both_scores(gtk_app, 0, 0);

	// vbox holds HBox
	gtk_box_pack_start(GTK_BOX(gtk_app->vbox), gtk_app->hbox,
			TRUE, TRUE, 0);

	gtk_widget_show_all(gtk_app->window);
}


/** 
 * create the pix buffer from file
 * 
 * @param filename 
 * 
 * @return a pointer to GdkPixbuf
 */
GdkPixbuf* create_pixbuf(const gchar* filename)
{
	GdkPixbuf* pixbuf;
	GError* error = NULL;

	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}

/** 
 * if the game complete, reminder user of the result and exit.
 * 
 * @param gtk_app 
 */
void game_is_over(gtk_app_t* gtk_app)
{
	static char* str[3] = { "player", "computer", "tie" };
	int idx;

	if (gtk_app->player_score_val == gtk_app->computer_score_val)
		idx = 2;
	else if (gtk_app->player_score_val > gtk_app->computer_score_val)
		idx = 0;
	else
		idx = 1;

	GtkWidget* dialog = gtk_message_dialog_new(
			GTK_WINDOW(gtk_app->window),
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
			"game is over, %s is winner\n", str[idx]);

	g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	gtk_widget_destroy(gtk_app->window);
}
