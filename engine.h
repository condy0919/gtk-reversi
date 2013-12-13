#ifndef ENGINE_H
#define ENGINE_H

#include "commondefs.h"
#include "list.h"

#include <time.h>
#include <stdlib.h>
#include <stdint.h>

// the weight for evaluation function
int board_weight[YMAX][XMAX];


// the coeff of the alpha-beta
int coeff;

/** 
 * check whether the position generate at least solution
 *
 * @return 
 */
int move_possible(int board[YMAX][XMAX], int x, int y, int color);

/** 
 * computer evaluation
 * 
 */
void computer_turn(int board[YMAX][XMAX], int recommend_depth, 
		int white_score, int black_score, int* x, int* y);


/** 
 * set up board weight for evaluation 
 * 
 */
void setup_board_weight();

/** 
 * evaluation function
 * 
 * 
 * @return the value of current board color
 */
int evaluate(int board[YMAX][XMAX], int color);


/*
 * liner combination evaluation function for current board
 */
int calc_value(int board[YMAX][XMAX], int self_color);


/** 
 * negamax search with alpha-beta pruning
 * 
 * @return the optimized position
 */
int alpha_beta(int board[YMAX][XMAX], int alpha, int beta, int depth, 
		int color, int* pos);


/*
 * minimax search whth alpha-beta pruning
 * just test
 */
int minimax(int board[YMAX][XMAX], int alpha, int beta, int depth, 
		int color, int* pos);


#endif
