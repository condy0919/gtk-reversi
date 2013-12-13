/*
 * the engine produces moves from the current board through calling to the 
 * function alpha_beta()
 *
 * alpha_beta() use nega-max algorithm with alpha-beta pruning as showed.
 * at every leaf node at the search tree, the resulting position is 
 * evaluated. two things are considered when evaluating a position: the 
 * number of pieces of each color at which squares the pieces are located.
 * pieces at the corner are valuable and give a high value, and having 
 * pieces at squares nect to a cornet is not very good and they give a lower
 * value. in te beginning of a game it is more important to have pieces on 
 * the "good" squares, but towards teh end the total number of pieces of 
 * each color is given a higher weight. other things, like how many legal 
 * moves that can be made in a position, and the number of pieces that can 
 * never be turned would probably make the program stronger if they were 
 * considered in evaluating a postion, but that would make thing more 
 * complicated.
 *
 * the board_weight holds board control value for each squares and is 
 * initiated by a call to the function setup_board_weight() . it is used in 
 * evaluation of position except when the game tree is searched all the way 
 * to the end of the game.
 *
 * list_pos is list storing all possible moves at the first level and the 
 * values that were calcualted for them. this makes it possible to select a 
 * random move among those with equal value after the search complete.
 */

#include "engine.h"

/** 
 * calcualte the possible solutions
 * 
 * 
 * @return 
 */
int move_possible(int board[YMAX][XMAX], int x, int y, int color)
{
	int dx, dy;
	int _x, _y;
	int cnt;

	if (board[y][x] != NO_COLOR)
		return 0;

	for (dy = -1; dy <= 1; ++dy)
		for (dx = -1; dx <= 1; ++dx) {
			if (dx == 0 && dy == 0)
				continue;

			cnt = 0;
			_x = x + dx, _y = y + dy;
			while (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
					board[_y][_x] == 1 - color) {
				_x += dx, _y += dy;
				++cnt;
			}
			if (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
					board[_y][_x] == color && cnt > 0)
				return 1;
		}
	return 0;
}

/** 
 * negemax search with alpha-beta pruning 
 * 
 * @return the optimized position
 */
int alpha_beta(int board[YMAX][XMAX], int alpha, int beta, int depth, 
		int color, int* pos)
{
	int dx, dy;
	int _x, _y;
	int i, j;
	int ret, temp;
	int idx;
	int res;
	list_t S, /* use as a stack */
		   list_pos;
	point_t point;

	// evaluate
	if (depth <= 0) {
		ret = calc_value(board, color);
		return ret;
	}

	init_list(&list_pos);
	// search all pieces
	for (i = 0; i < YMAX && alpha < beta; ++i)
		for (j = 0; j < XMAX && alpha < beta; ++j) {
			if (board[i][j] != NO_COLOR)
				continue;

			init_list(&S);
			// search surrounding pieces
			for (dy = -1; dy <= 1; ++dy)
				for (dx = -1; dx <= 1; ++dx) {
					if (dx == 0 && dy == 0)
						continue;

					_x = j + dx, _y = i + dy;
					while (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
							board[_y][_x] == 1 - color)
						_x += dx, _y += dy;

					if (_x >= 0 && _x < XMAX && _y >= 0 && _x < YMAX &&
							board[_y][_x] == color)
						for (_x -= dx, _y -= dy; _x != j || _y != i; _x -= dx, _y -= dy) {
							board[_y][_x] = color;
							point.y = _y, point.x = _x;
							push(&S, point);
						}
				}
			if (empty(&S))
				continue;

			res = -alpha_beta(board, -beta, -alpha, depth - 1, 1 - color, &temp);

			// resume the turned pieces
			while (!empty(&S)) {
				point = top(&S);
				pop(&S);

				_x = point.x, _y = point.y;
				board[_y][_x] = 1 - color;
			}

			// alpha beta judge
			if (res > alpha) {
				alpha = res;

				point.x = j;
				point.y = i;

				init_list(&list_pos);
				push(&list_pos, point);
			} else if (res == alpha) {
				point.x = j;
				point.y = i;
				push(&list_pos, point);
			}
		}

	// at least 1 solution, pick one randomly
	if (!empty(&list_pos)) {
		idx = rand() % list_pos.size;
		*pos = list_pos.s[idx].y * 8 + list_pos.s[idx].x;
	}

	return alpha;
}


/*
 * only work correctly in YMAX * XMAX array
 */
int minimax(int board[YMAX][XMAX], int alpha, int beta, int depth, 
		int color, int* pos)
{
	int ret;
	int i, j;
	int _x, _y;
	int dx, dy;
	int temp;
	list_t list_pos, S;
	point_t point;

	if (depth <= 0) {
		ret = evaluate(board, color);
		return ret;
	}

	init_list(&list_pos);
	if (color == WHITE) {// max player
		// search all pieces
		for (i = 0; i < 8 && alpha < beta; ++i)
			for (j = 0; j < 8 && alpha < beta; ++j) {
				if (board[i][j] != NO_COLOR)
					continue;

				init_list(&S);
				// search surrounding pieces
				for (dy = -1; dy <= 1; ++dy)
					for (dx = -1; dx <= 1; ++dx) {
						if (dx == 0 && dy == 0)
							continue;

						_x = j + dx, _y = i + dy;
						while (_x >= 0 && _x < XMAX && _y >= 0 &&
								_y < YMAX && board[_y][_x] == 1 - color)
							_x += dx, _y += dy;

						if (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
								board[_y][_x] == color)
							for (_x -= dx, _y -= dy; _x != j || _y != i; _x -= dx, _y -= dy) {
								board[_y][_x] = color;
								point.y = _y, point.x = _x;
								push(&S, point);
							}

					}
				if (empty(&S))
					continue;
				ret = minimax(board, alpha, beta, depth - 1, 1 - color, &temp);

				// resume 
				while (!empty(&S)) {
					point = top(&S);
					pop(&S);

					_x = point.x, _y = point.y;
					board[_y][_x] = 1 - color;
				}

				// judge
				if (ret > alpha) {
					alpha = ret;
					point.x = j, point.y = i;

					init_list(&list_pos);
					push(&list_pos, point);
				} else if (ret == alpha) {
					point.x = j, point.y = i;
					push(&list_pos, point);
				}
			}
		if (!empty(&list_pos)) {
			int idx = rand() % list_pos.size;
			*pos = 8 * list_pos.s[idx].y + list_pos.s[idx].x;
		}
		return alpha;
	} 

	// min player
	// search all pieces
	for (i = 0; i < 8 && alpha < beta; ++i)
		for (j = 0; j < 8 && alpha < beta; ++j) {
			if (board[i][j] != NO_COLOR)
				continue;

			init_list(&S);
			// search surrounding pieces
			for (dy = -1; dy <= 1; ++dy)
				for (dx = -1; dx <= 1; ++dx) {
					if (dx == 0 && dy == 0)
						continue;

					_x = j + dx, _y = i + dy;
					while (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
							board[_y][_x] == 1 - color)
						_x += dx, _y += dy;

					if (_x >= 0 && _x < XMAX && _y >= 0 && _y < YMAX &&
							board[_y][_x] == color)
						for (_x -= dx, _y -= dy; _x != j || _y != i; _x -= dx, _y -= dy) {
							board[_y][_x] = color;
							point.y = _y, point.x = _x;
							push(&S, point);
						}
				}
			if (empty(&S))
				continue;
			ret = minimax(board, alpha, beta, depth - 1, 1 - color, &temp);

			// resume 
			while (!empty(&S)) {
				point = top(&S);
				pop(&S);

				_x = point.x, _y = point.y;
				board[_y][_x] = 1 - color;
			}

			if (ret < beta) {
				beta = ret;
				point.x = j, point.y = i;

				init_list(&list_pos);
				push(&list_pos, point);
			} else if (ret == beta) {
				point.x = j, point.y = i;
				push(&list_pos, point);
			}

		}
	if (!empty(&list_pos)) {
		int idx = rand() % list_pos.size;
		*pos = 8 * list_pos.s[idx].y + list_pos.s[idx].x;
	}
	return beta;
}


/** 
 * computer turn to get the best answer
 *
 * using nega-search with alpha-beta pruning
 * 
 */
void computer_turn(int board[YMAX][XMAX], int recommend_depth, int white_score, int black_score, int* x, int* y)
{
	int pos, ret;
	int depth = (recommend_depth == 0 ? 7 : 1);

	// consider the corner
	if (move_possible(board, 0, 0, WHITE)) {
		*x = 0, *y = 0;
		return;
	} else if (move_possible(board, 0, 7, WHITE)) {
		*x = 0, *y = 7;
		return;
	} else if (move_possible(board, 7, 0, WHITE)) {
		*x = 7, *y = 0;
		return;
	} else if (move_possible(board, 7, 7, WHITE)) {
		*x = 7, *y = 7;
		return;
	}

	// determine the coeff value 
	coeff = 100 - (100 * (evaluate(board, WHITE) + evaluate(board, BLACK) + depth - 4)) / 60;

	// if it is close to end, search more
	if (white_score + black_score + depth + 3 >= XMAX * YMAX)
		depth = XMAX * YMAX - white_score - black_score;
	else if (white_score + black_score + depth + 4 >= XMAX * YMAX)
		depth += 2;
	else if (white_score + black_score + depth + 5 >= XMAX * YMAX)
		++depth;

	ret = alpha_beta(board, -ILLEGAL_NUMBER, ILLEGAL_NUMBER, depth, WHITE, &pos);

	//ret = minimax(board, -ILLEGAL_NUMBER, ILLEGAL_NUMBER, depth, 
	//		WHITE, &pos);
	// no solution
	if (ret == -ILLEGAL_NUMBER)
		return;
	*x = pos % 8;
	*y = pos / 8;
}


/*
 * set up the board control value tht be used in evaluation
 * of the postion
 */
void setup_board_weight()
{
	int i, j;

	for (i = 0; i < YMAX; ++i)
		for (j = 0; j < XMAX; ++j) {
			if (i == 1 || i == 6)
				board_weight[i][j] = -1;
			else
				board_weight[i][j] = 0;
			if (j == 2 || j == 5)
				board_weight[i][j] -= 1;
		}

	board_weight[0][0] = 2;
	board_weight[7][0] = 2;
	board_weight[0][7] = 2;
	board_weight[7][7] = 2;

	board_weight[0][1] = -1;
	board_weight[1][0] = -1;
	board_weight[0][6] = -1;
	board_weight[6][0] = -1;
	board_weight[7][1] = -1;
	board_weight[1][7] = -1;
	board_weight[7][6] = -1;
	board_weight[6][7] = -1;
}


/** 
 * evaluate the value of the color using the board weight
 * 
 * 
 * @return 
 */
int evaluate(int board[YMAX][XMAX], int color)
{
	int sum = 0;
	int i, j;

	for (i = 0; i < YMAX; ++i)
		for (j = 0; j < XMAX; ++j)
			if (board[i][j] == color)
				sum += board_weight[i][j];
	return sum;
}


/** 
 * evaluatation function for the current board.
 * 
 * @return evaluated value
 */
int calc_value(int board[YMAX][XMAX], int self_color)
{
	int ret;
	int opponent = 1 - self_color;
	int self_score, opponent_score;

	self_score = evaluate(board, self_color);
	opponent_score = evaluate(board, opponent);

	// I don't know the principle
	ret = (100 - coeff) * (self_score - opponent_score) + 
		coeff * BC_WEIGHT * (self_score - opponent_score);

	return ret;
}
