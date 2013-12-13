#include "signal_callback.h"


// exit function for app window
void on_window_destroy(GtkObject* object, gtk_app_t* gtk_app)
{
	gtk_main_quit();
}


// build the whole appearance and init the random seed
int on_configure_event(GtkWidget* draw_area, GdkEventConfigure* event,
		gtk_app_t* gtk_app)
{
	int i;
	GError* error = NULL;
	GdkPixbuf* image = NULL;
	const gchar* pic_filename[5] = {
		"res/black.png", 
		"res/white.png",
		"res/bgcolor.png",
		"res/vlabel.png",
		"res/hlabel.png"
	};

	// init random seed
	srand(time(NULL));

	for (i = 0; i < 5; ++i) {
		// load piece image
		image = gdk_pixbuf_new_from_file(pic_filename[i], &error);
		if (error) {
			g_warning(G_STRLOC ": load image error:%s\n", error->message);
			error = NULL;
		}

		// convert GdkPixbuf to GdkPixmap
		gdk_pixbuf_render_pixmap_and_mask_for_colormap(image,
				gdk_colormap_get_system(),
				&gtk_app->piece[i],
				&gtk_app->piece_mask[i],
				0x7f);
	}

	if (gtk_app->pixmap)
		g_object_unref(gtk_app->pixmap);

	// create a new pixmap
	gtk_app->pixmap = gdk_pixmap_new(draw_area->window,
			draw_area->allocation.width, 
			draw_area->allocation.height,
			-1);


	// draw background image rectangle
	gdk_draw_drawable(gtk_app->pixmap,
			draw_area->style->fg_gc[GTK_WIDGET_STATE(draw_area)],
			gtk_app->piece[2],
			0, 0, 0, 0,
			draw_area->allocation.width, 
			draw_area->allocation.height);


	// draw vertical left label image (A B C...)
	gdk_draw_drawable(gtk_app->pixmap,
			draw_area->style->fg_gc[GTK_WIDGET_STATE(draw_area)],
			gtk_app->piece[3], 
			0, 0, 0, YOFFSET + YBORDER, XOFFSET, BOARD_HEIGHT);

	// draw vertical right label image
	gdk_draw_drawable(gtk_app->pixmap,
			draw_area->style->fg_gc[GTK_WIDGET_STATE(draw_area)],
			gtk_app->piece[3],
			0, 0, XOFFSET + 2 * XBORDER + BOARD_HEIGHT, 
			YOFFSET + YBORDER, XOFFSET, BOARD_HEIGHT);


	// draw horizontal top label image (1 2 3...)
	gdk_draw_drawable(gtk_app->pixmap, 
			draw_area->style->fg_gc[GTK_WIDGET_STATE(draw_area)],
			gtk_app->piece[4],
			0, 0, XOFFSET + XBORDER, 0, BOARD_WIDTH, YOFFSET);

	// draw horizontal bottom label image
	gdk_draw_drawable(gtk_app->pixmap,
			draw_area->style->fg_gc[GTK_WIDGET_STATE(draw_area)],
			gtk_app->piece[4], 
			0, 0,
			XOFFSET + XBORDER,
			YOFFSET + 2 * YBORDER + BOARD_HEIGHT,
			BOARD_WIDTH, YOFFSET);

	// draw outer border
	gdk_draw_rectangle(gtk_app->pixmap,
			draw_area->style->black_gc, 
			FALSE, 
			0, 0, 
			BOARD_WIDTH + 2 * XBORDER + 2 * XOFFSET - 1, 
			BOARD_HEIGHT + 2 * YBORDER + 2 * YOFFSET - 1);

	gdk_draw_rectangle(gtk_app->pixmap,
			draw_area->style->black_gc,
			FALSE, 
			XOFFSET, YOFFSET,
			BOARD_WIDTH + 2 * XBORDER - 1, 
			BOARD_HEIGHT + 2 * YBORDER - 1);

	gdk_draw_rectangle(gtk_app->pixmap,
			draw_area->style->black_gc,
			FALSE,
			XOFFSET - 1, YOFFSET - 1,
			BOARD_WIDTH + 2 * XBORDER + 1, 
			BOARD_HEIGHT + 2 * YBORDER + 1);

	// draw inner border
	gdk_draw_rectangle(gtk_app->pixmap,
			draw_area->style->black_gc,
			FALSE, 
			XOFFSET + XBORDER, YOFFSET + YBORDER,
			BOARD_WIDTH - 1, BOARD_HEIGHT - 1);

	// draw grid
	for (i = 1; i < XMAX; ++i) {
		// vertical line
		gdk_draw_line(gtk_app->pixmap,
				draw_area->style->black_gc,
				XOFFSET + XBORDER + i * BOARD_WIDTH / XMAX - 1,
				YOFFSET + YBORDER,
				XOFFSET + XBORDER + i * BOARD_WIDTH / XMAX - 1,
				YOFFSET + YBORDER + BOARD_HEIGHT - 1);

		// horizontal line
		gdk_draw_line(gtk_app->pixmap,
				draw_area->style->black_gc,
				XOFFSET + XBORDER,
				YOFFSET + YBORDER + i * BOARD_HEIGHT / YMAX - 1,
				XOFFSET + XBORDER + BOARD_WIDTH - 1,
				YOFFSET + YBORDER + i * BOARD_HEIGHT / YMAX - 1);
	}

	g_object_unref(image);

	return 0;
}

int on_expose_event(GtkWidget* draw_area, GdkEventExpose* event, gtk_app_t* gtk_app)
{
	show_pixmap(gtk_app);
	return 0;
}

/*
 * player game procedure
 */
int on_button_press_event(GtkWidget* draw_area, GdkEventButton* event, gtk_app_t* gtk_app)
{
	int x, y;
	int tx, ty;
	int temp;

	// out of range
	if (event->x - XOFFSET - XBORDER < 0 ||
			event->y - YOFFSET - YBORDER < 0 ||
			event->x - XOFFSET - XBORDER > BOARD_WIDTH ||
			event->y - YOFFSET - YBORDER > BOARD_HEIGHT)
		return 0;

	x = (event->x - XOFFSET - XBORDER) / BOARD_WIDTH * XMAX;
	y = (event->y - YOFFSET - YBORDER) / BOARD_WIDTH * YMAX;

	printf("x = %d, y = %d\n", x, y);
	/*
	 * event->button: 
	 * 1 left-click
	 * 2 middile-click
	 * 3 right-click
	 */
	if (event->button != 1 && event->button != 3)
		return 0;
	if (!move_possible(gtk_app->board, x, y, BLACK)) {
		if (DEBUG) {
			fprintf(stderr, "player has no solution\n");
			//sleep(5);// DEBUG
		}
		return 0;
	}

	// refresh both scores
	set_color(gtk_app, x, y, BLACK);
	++gtk_app->placed_number;
	++gtk_app->player_score_val;

	// turn around pieces
	turn_around_pieces(gtk_app, x, y, BLACK);

	update_both_scores(gtk_app, gtk_app->player_score_val, 
			gtk_app->computer_score_val);

	// reach the limitation
	if (gtk_app->placed_number == XMAX * YMAX) {
		game_is_over(gtk_app);
		return 0;
	}

	show_pixmap(gtk_app);

	do {// never stop untile player have a solution

		// some timeouts for comfort.
		//while (gtk_events_pending())
		//	gtk_main_iteration();
		//sleep(1);

		// call computer to game with player
		tx = ty = -1;
		computer_turn(gtk_app->board, 0, gtk_app->computer_score_val, gtk_app->player_score_val, &tx, &ty);
		// no solution
		if (tx == -1 && ty == -1) {
			if (DEBUG) {
				fprintf(stderr, "computer has no solution\n");
				//sleep(5);
			}

			// check whether player can continue playing
			if (alpha_beta(gtk_app->board, -ILLEGAL_NUMBER, ILLEGAL_NUMBER, 1, BLACK, &temp) == -ILLEGAL_NUMBER) {
				if (DEBUG) {
					fprintf(stderr, "player has no solution\n");
					//sleep(5);
				}
				// game over, judge by the current score
				game_is_over(gtk_app);
			}
			return 1;
		}

		set_color(gtk_app, tx, ty, WHITE);
		++gtk_app->placed_number;
		++gtk_app->computer_score_val;

		// turn available pieces
		turn_around_pieces(gtk_app, tx, ty, WHITE);

		update_both_scores(gtk_app, gtk_app->player_score_val, gtk_app->computer_score_val);

		// reach the limitation
		if (gtk_app->placed_number == XMAX * YMAX) {
			game_is_over(gtk_app);
			return 0;
		}

		show_pixmap(gtk_app);
	} while (alpha_beta(gtk_app->board, -ILLEGAL_NUMBER, ILLEGAL_NUMBER, 1, BLACK, &temp) == -ILLEGAL_NUMBER);

	return 1;
}

void show_pixmap(gtk_app_t* gtk_app)
{
	gdk_draw_drawable(gtk_app->draw_area->window,
			gtk_app->draw_area->style->fg_gc[GTK_WIDGET_STATE(gtk_app->draw_area)],
			gtk_app->pixmap,
			0, 0, 0, 0,
			gtk_app->draw_area->allocation.width,
			gtk_app->draw_area->allocation.height);
}


/*
 * initialize the board and game starts. the computer first.
 */
void on_start_button_click(GtkButton* button, gtk_app_t* gtk_app)
{
	int i, j;
	int x, y;

	// set up board weigth
	setup_board_weight();
	if (DEBUG) {
		fprintf(stderr, "the board weigth :\n");
		for (i = 0; i < YMAX; ++i, fputc('\n', stderr))
			for (j = 0; j < XMAX; ++j)
				fprintf(stderr, "%4d", board_weight[i][j]);
	}

	// clean the whole board
	for (i = 0; i < XMAX; ++i)
		for (j = 0; j < YMAX; ++j)
			set_color(gtk_app, i, j, NO_COLOR);

	update_both_scores(gtk_app, 0, 0);

	set_color(gtk_app, 3, 3, BLACK);
	set_color(gtk_app, 4, 4, BLACK);
	set_color(gtk_app, 3, 4, WHITE);
	set_color(gtk_app, 4, 3, WHITE);
	gtk_app->player_score_val = gtk_app->computer_score_val = 2;
	gtk_app->placed_number = 4;

	show_pixmap(gtk_app);

	// some timeouts for comfort.
	//while (gtk_events_pending())
	//	gtk_main_iteration();
	//sleep(1);

	// game starts, it must have solution
	computer_turn(gtk_app->board, 1, gtk_app->computer_score_val, gtk_app->player_score_val, &x, &y);
	set_color(gtk_app, x, y, WHITE);
	++gtk_app->placed_number;
	++gtk_app->computer_score_val;
	turn_around_pieces(gtk_app, x, y, WHITE);

	update_both_scores(gtk_app, gtk_app->player_score_val, gtk_app->computer_score_val);

	show_pixmap(gtk_app);
}

