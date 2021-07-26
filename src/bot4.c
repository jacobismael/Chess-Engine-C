//first decent bvot uses raw brute force
#include "bot4.h"


//this needs to be odd I thyink
#define DEPTH 7

/*


	x--data--o---o---o---o---o
	|
   next
	|
	x--o---o---o---o---o
	|
	|
	x--o---o---o---o---o
	|
	|
	x--o---o---o---o---o
*/

struct fullDataTurn *getBest(const struct dataBoard *input_board, const char side) {
	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));
	struct standardList *all_moves = allLegalMoves(input_board, side);
	struct standardList *all_moves_cpy = all_moves;
	struct standardList *built_list = NULL;
	struct standardList *list_to_free = NULL;

	float best_score = side == 'W' ? -INFINITY : INFINITY;

	struct fullDataTurn *result = malloc(sizeof(struct fullDataTurn));
	bool status = true;
	struct boardMove *temp = malloc(sizeof(struct boardMove));
	while(all_moves != NULL) {
		memcpy(copy_board, input_board, sizeof(struct dataBoard));
		temp->turn = *(struct fullDataTurn*)all_moves->data;
		temp->board = *buildFromHalfMove(copy_board, &temp->turn, side, &status) ;

		temp->score = getBoardScore(copy_board);
		built_list = prependToStandardList(built_list, temp);
		generateListofMoveForStartingMove(built_list, oppositeSide(side), 0);
		list_to_free = built_list;
		//get to last element of array
		while (built_list->next !=  NULL) { 
			built_list = built_list->next;
		}
		//if its better set it as the best
		if (((struct boardMove*)built_list->data)->score > best_score) {
			*result = *(struct fullDataTurn*)all_moves->data;
			best_score = (float)((struct boardMove*)built_list->data)->score;
			printf("score: %f\n", best_score);
		}
		//try next move 
		all_moves = all_moves->next;
	}
	freeStandardList(all_moves_cpy);

	return result;
}

void generateListofMoveForStartingMove(struct standardList *head, const char side, unsigned short depth_reached) {
	if (depth_reached >= DEPTH) {
		return;
	}
	bool status = 1;
	float best_so_far = side == 'W' ? -INFINITY : INFINITY;
	struct dataBoard editable_board; 
	struct boardMove *test = head->data;
	editable_board = test->board;
	struct standardList *all_moves = allLegalMoves(&editable_board, side);
	if (all_moves == NULL) {
		return;
	}
	struct standardList *all_moves_cpy = all_moves;
	struct boardMove *best = malloc(sizeof(struct boardMove)); 
	while (all_moves != NULL) {
		editable_board = ((struct boardMove*)head->data)->board;
		editable_board = *buildFromHalfMove(
			&editable_board,
			(struct fullDataTurn*)all_moves->data,
			side, &status
		);

		if ((getBoardScore(&editable_board) > best_so_far && side == 'W') || (getBoardScore(&editable_board) < best_so_far && side == 'B')) {
			best_so_far = getBoardScore(&editable_board);
			best->board = editable_board;
			best->score = best_so_far;
			best->turn = *(struct fullDataTurn*)all_moves->data;
		}
		all_moves = all_moves->next;
	}
	freeStandardList(all_moves_cpy);
	head = appendToStandardListReturnLast(head, best);
	generateListofMoveForStartingMove(head, oppositeSide(side), depth_reached + 1);
	return;
}


struct fullDataTurn *bot4Choice(const struct dataBoard *input_board, const char side, bool *status) {
	*status = true;
	return getBest(input_board, side);
}
