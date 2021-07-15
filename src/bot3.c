//first decent bvot uses raw brute force
#include "bot3.h"

#define DEPTH 1


struct fullDataTurn* bot3Choice(const struct dataBoard* input_board, const char side, bool* status) {
	struct standardList const* all_moves_list = allLegalMoves(input_board, side);

	float highest_score = -INFINITY;
	struct standardList* best_nodes = NULL;
	struct fullDataTurn* best = NULL;
	*status = true;
	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));

	struct standardList const* current_move = all_moves_list;
	while (current_move != NULL && current_move->next != NULL) {
		struct fullDataTurn* temp = (struct fullDataTurn*)current_move->data;

		memcpy(copy_board, input_board, sizeof(struct dataBoard));
		buildFromHalfMove(copy_board, (current_move->data), side, status);
		float board_score = getBasicBoardScore(copy_board);
		if (board_score == highest_score) {
			best_nodes = prependToStandardList(best_nodes, current_move->data);
			// printf("Prepending to best_nodes:\n");
		}
		else if (board_score > highest_score || (board_score == highest_score && kingExists(copy_board, oppositeSide(side)) && kingInCheck(copy_board, oppositeSide(side)))) {
			highest_score = board_score;
			freeLinkedList(best_nodes);
			best_nodes = NULL;

			best_nodes = prependToStandardList(best_nodes, current_move->data);
			// printf("Prepending to best_nodes:\n");
		}
		else if(kingExists(copy_board, oppositeSide(side)) && kingInCheck(copy_board, oppositeSide(side))) {
			highest_score++;
			freeLinkedList(best_nodes);
			best_nodes = NULL;

			best_nodes = prependToStandardList(best_nodes, current_move->data);
		}
		
		current_move = current_move->next;
    }
	int random_choice = randomInt(0, lengthOfLinkedList(best_nodes) - 1) ;
	struct standardList* temp = NULL;
	if (best_nodes != NULL) {
		temp = getElementOfLinkedList(best_nodes, random_choice);
	}
	best = (struct fullDataTurn*)temp->data;

	*status = true;
	return best;
}
