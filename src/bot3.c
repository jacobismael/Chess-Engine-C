//first decent bvot uses raw brute force
#include "bot3.h"

#define DEPTH 1


struct fullDataTurn *bot3Choice(const struct dataBoard *input_board, const char side, bool *status) {
	struct standardList *all_moves_list = allLegalMoves(input_board, side);

	float highest_score = -INFINITY;
	struct standardList *best_nodes = NULL;
	struct fullDataTurn *best = malloc(sizeof(struct fullDataTurn));
	*status = true;
	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));

	struct standardList const *current_move = all_moves_list;
	while (current_move != NULL && current_move->next != NULL) {
		struct fullDataTurn *temp = (struct fullDataTurn*)current_move->data;

		memcpy(copy_board, input_board, sizeof(struct dataBoard));
		buildFromHalfMove(copy_board, (current_move->data), side, status);
		float board_score = getBasicBoardScore(copy_board);
		if (board_score == highest_score) {
			best_nodes = prependToStandardList(best_nodes, current_move->data);
			// printf("Prepending to best_nodes:\n");
		}
		else if (board_score > highest_score || (board_score == highest_score && kingExists(copy_board, oppositeSide(side)) && kingInCheck(copy_board, oppositeSide(side)))) {
			highest_score = board_score;
			//freeLinkedList(best_nodes);
			freeStandardListWithoutData(best_nodes);
			best_nodes = NULL;

			best_nodes = prependToStandardList(best_nodes, current_move->data);
			// printf("Prepending to best_nodes:\n");
		}
		else if(kingExists(copy_board, oppositeSide(side)) && kingInCheck(copy_board, oppositeSide(side))) {
			highest_score++;
			//freeLinkedList(best_nodes);
			freeStandardListWithoutData(best_nodes);
			best_nodes = NULL;

			best_nodes = prependToStandardList(best_nodes, current_move->data);
		}
		
		current_move = current_move->next;
    }
	free(copy_board);
	
	*status = true;
	int random_choice = randomInt(0, lengthOfStandardList(best_nodes) - 1);
	struct standardList *temp = NULL;
	if (best_nodes != NULL) {
		temp = getElementOfStandardList(best_nodes, random_choice);
	}
	else {
		temp = getElementOfStandardList(all_moves_list, randomInt(0, lengthOfStandardList(all_moves_list) - 1));
	}
	if (temp == NULL) {
		*status  = false;
		printf("length of all moves list is: %d\n", lengthOfStandardList(all_moves_list));
		return NULL;
	}
	memcpy(best, (struct fullDataTurn*)temp->data, sizeof(struct fullDataTurn));

	//freeLinkedList(best_nodes);
	freeStandardListWithoutData(best_nodes);
	freeStandardList(all_moves_list);
	return best;
}
