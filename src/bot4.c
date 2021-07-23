//first decent bvot uses raw brute force
#include "bot4.h"


//this needs to be odd I thyink
#define DEPTH 17

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

struct fullDataTurn *getBest(struct dataBoard *input_board, const char side) {
	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));
	struct standardList *all_moves = allLegalMoves(input_board, side);
//	struct standardList *list_of_lists = NULL;

	struct standardList *builtList = NULL;
	float best_score = side == 'W' ? -INFINITY : INFINITY;
	struct fullDataTurn *result;
	bool status = true;
	struct boardMove *temp = malloc(sizeof(struct boardMove));
	while(all_moves != NULL) {
		memcpy(copy_board, input_board, sizeof(struct dataBoard));
		temp->turn = (struct fullDataTurn*)all_moves->data;
		temp->board = buildFromHalfMove(copy_board, temp->turn, 'W', &status) ;
		//printf(":copy board:\n");
		//printDataBoard(copy_board, true);
		temp->score = getBoardScore(copy_board);
		builtList = prependToStandardList(builtList, temp);
		// printf(":copy board2:\n");
		// printDataBoard(copy_board, true);
		// printDataBoard(((struct boardMove*)builtList->data)->board, true);
		generateListofMoveForStartingMove(builtList, oppositeSide(side), 0);
		//get to last element of array
		while (builtList->next !=  NULL) { 
			builtList = builtList->next;
		}
		//if its better set it as the best
		if (((struct boardMove*)builtList->data)->score > best_score) {
			result = (struct fullDataTurn*)all_moves->data;
			best_score = (float)((struct boardMove*)builtList->data)->score;
			printf("score: %f\n", best_score);
		}
		//try next move 
		all_moves = all_moves->next;
	}

	return result;
}

void generateListofMoveForStartingMove(struct standardList *head, const char side, unsigned short depth_reached) {
	if (depth_reached >= DEPTH) {
	printf("test");
		return;
	}
	bool status = 1;
	float best_so_far = side == 'W' ? -INFINITY : INFINITY;
	struct dataBoard *editable_board = malloc(sizeof(struct dataBoard));
	struct boardMove* test = head->data;
	//printf("yeet: %f\n", (float)test->score);
	//printf("here\n");
//	printDataBoard(test, true);
	//printDataBoard(((struct boardMove*)head->data)->board, true);
	memcpy(editable_board, test->board, sizeof(struct dataBoard));
	//printDataBoard(editable_board, true);
	struct standardList *all_moves = allLegalMoves(editable_board, side);
	if (all_moves == NULL) {
		return;
	}
	struct boardMove *best = malloc(sizeof(struct boardMove)); 
	unsigned char multiplier = (side == 'W' ? -1 : 1);
	while (all_moves != NULL) {
		memcpy(editable_board, ((struct boardMove*)head->data)->board, sizeof(struct dataBoard));
		editable_board = buildFromHalfMove(
			editable_board,
			(struct fullDataTurn*)all_moves->data,
			side, &status);

		if ((getBoardScore(editable_board) > best_so_far && side == 'W') || (getBoardScore(editable_board) < best_so_far && side == 'B')) {
			best_so_far = getBoardScore(editable_board);
			best->board = editable_board;
			best->score = best_so_far;
			best->turn = (struct fullDataTurn*)all_moves->data;
			// if (abs(best_so_far) == INFINITY && depth_reached == DEPTH - 1) {
			// 	printf("here\n");

			// 	head = appendToStandardListReturnLast(head, best);
			// 	generateListofMoveForStartingMove(head, oppositeSide(side), depth_reached + 1);
			// }

		}
		all_moves = all_moves->next;
	}

	head = appendToStandardListReturnLast(head, best);

	generateListofMoveForStartingMove(head, oppositeSide(side), depth_reached + 1);
	
}


struct fullDataTurn *bot4Choice(struct dataBoard *input_board, const char side, bool *status) {
	*status = true;
	return getBest(input_board, side);
}
