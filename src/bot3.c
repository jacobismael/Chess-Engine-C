//first decent bvot uses raw brute force
#include "bot3.h"

#define depth =  1

struct fullDataTurn* bot3Choice(const struct dataBoard* input_board, char side, bool* status) {
	struct fullDataTurnNode* head; // en passant might not work
	head = allLegalMoves(input_board, side);
	int index = 0;
	float highest_score = -INFINITY;
	struct fullDataTurn* best;
	*status = true;	
	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));
	while (head != NULL) {
		memcpy(copy_board, input_board, sizeof(struct dataBoard));
		buildFromHalfMove(copy_board, fullDataTurnNodeTofullDataTurn(head), side, status);
		if (getBasicBoardScore(copy_board) > highest_score) {
			highest_score = getBasicBoardScore(copy_board);
			best = fullDataTurnNodeTofullDataTurn(head);
		}
		printf("score %f\n", highest_score);
		index++;
		
		head = head->next;
	}
	*status = true;
	
	return best;
}
