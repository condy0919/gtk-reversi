#ifndef COMMONDEFS_H
#define COMMONDEFS_H

#include <limits.h>

// DEBUG MODE
#ifndef DEBUG
#define DEBUG 1
#endif

#define YMAX 8
#define XMAX 8

#define TILE_WIDTH 40
#define TILE_HEIGHT 40

#define XOFFSET 30
#define YOFFSET 30

#define XBORDER (TILE_WIDTH/2)
#define YBORDER (TILE_HEIGHT/2)

#define BOARD_WIDTH (XMAX * TILE_WIDTH)
#define BOARD_HEIGHT (YMAX * TILE_HEIGHT)

#define GRID_WIDTH 1

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

// color defination
#define BLACK 0
#define WHITE 1
#define NO_COLOR 2

// used for alpha-beta pruning
#define ILLEGAL_NUMBER INT_MAX

#define BC_WEIGHT 3

/**
 * @file   commondefs.h
 * @author condy chen <condy0919@gmail.com>
 * @date   Tue Dec 10 23:10:17 2013
 * 
 * @brief  define the gtk_app_t type, which contains all Widget
 * 
 */


#include <gtk/gtk.h>

typedef struct point_t {
	int x, y;
} point_t;

typedef struct gtk_app_t {
	// whole app window
	GtkWidget* window;// holds all
	GtkWidget* vbox;// holds draw_area and hbox
	GtkWidget* hbox;// holds 2 labels and start button

	// start button
	GtkWidget* start_button;

	// the drawable board 
	GtkWidget* draw_area;
	int board[YMAX][XMAX];
	int placed_number;


	// competition info
	int player_score_val, computer_score_val;
	GtkWidget* player_score;
	GtkWidget* computer_score;

	// current color info
	GdkPixmap* pixmap;

	// pics loaded from file.
	// piece[0]: black ball
	// piece[1]: white ball
	// piece[2]: back ground pic
	// piece[3]: vertical label
	// piece[4]: horizontal lable
	GdkPixmap* piece[5];
	GdkPixmap* piece_mask[5];
} gtk_app_t;

#endif
