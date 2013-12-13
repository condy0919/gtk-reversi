#include "interface.h"

/*
 * assign the (x, y) postition with color 
 */
void set_color(gtk_app_t* gtk_app, int x, int y, int color)
{
	if (color < 0 || color > 2) {
		printf("color is out of range.\n");
		return;
	}

	// in the draw area
	gdk_draw_drawable(gtk_app->pixmap,
			gtk_app->draw_area->style->fg_gc[\
			GTK_WIDGET_STATE(gtk_app->draw_area)],
			gtk_app->piece[color],
			0, 0, 
			XOFFSET + XBORDER + GRID_WIDTH + x * TILE_WIDTH,
			YOFFSET + YBORDER + GRID_WIDTH + y * TILE_HEIGHT,
			TILE_WIDTH - 2 * GRID_WIDTH,
			TILE_HEIGHT - 2 * GRID_WIDTH);

	// in the board in data
	gtk_app->board[y][x] = color;
}

/*
 * refresh the player score label and the computer score label
 */
void update_both_scores(gtk_app_t* gtk_app, int player, int computer)
{
	 gchar label[128], number[128];

	 gtk_app->player_score_val = player;
	  // player:
	 strcpy(label, "player: ");
	 sprintf(number, "%d", player);
	 strcat(label, number);
	 gtk_label_set_label(GTK_LABEL(gtk_app->player_score), label);
	  
	 gtk_app->computer_score_val = computer;
	 // computer:
	 strcpy(label, "computer: ");
	 sprintf(number, "%d", computer);
	 strcat(label, number);
	 gtk_label_set_label(GTK_LABEL(gtk_app->computer_score), label);
}


/*
 * turn over the pieces which obey rules
 */
void turn_around_pieces(gtk_app_t* gtk_app, int x, int y, int color)
{
	int dx, dy;
	int _x, _y;
	int cnt = 0;
	point_t point;
	list_t list_pos;

	init_list(&list_pos);

	for (dy = -1; dy <= 1; ++dy)
		for (dx = -1; dx <= 1; ++dx) {
			if (dx == 0 && dy == 0)
				continue;

			_x = x + dx, _y = y + dy;
			while (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
					gtk_app->board[_y][_x] == 1 - color) {
				point.x = _x, point.y = _y;
				push(&list_pos, point);
				_x += dx, _y += dy;
			}
			if (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
					gtk_app->board[_y][_x] == color)// successful
				while (!empty(&list_pos)) {
					point = top(&list_pos);
					pop(&list_pos);
					gtk_app->board[point.y][point.x] = color;
					++cnt;
					set_color(gtk_app, point.x, point.y, color);
				}
			else // clear the unsuccessful pieces
				while (!empty(&list_pos))
					pop(&list_pos);
		}

	if (color == WHITE) {
		gtk_app->player_score_val -= cnt;
		gtk_app->computer_score_val += cnt;
	} else {
		gtk_app->computer_score_val -= cnt;
		gtk_app->player_score_val += cnt;
	}
	update_both_scores(gtk_app, gtk_app->player_score_val,
			gtk_app->computer_score_val);
}

