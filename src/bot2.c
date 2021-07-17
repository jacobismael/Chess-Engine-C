//random-bot-improved
#include "bot2.h"


struct standardList *getOnlyAttackingMoves(struct standardList *head, const struct dataBoard *input_board) {
	struct standardList *new_list_head = malloc(sizeof(struct standardList));
	new_list_head->next = NULL;

	while (head != NULL) {
		if (doesTake(input_board, &((struct basicDataTurn*)head->data)->starting_pos, &((struct basicDataTurn*)head->data)->ending_pos)) {
			struct basicDataTurn *new_move = malloc(sizeof(struct basicDataTurn));
			new_move->starting_pos = ((struct basicDataTurn*)head->data)->starting_pos;
			new_move->ending_pos = ((struct basicDataTurn*)head->data)->ending_pos;
			new_list_head = prependToStandardList(new_list_head, new_move);
		}
		head = head->next;
	}
	struct standardList *result = new_list_head->next;
	free(new_list_head);
	return result;
}


struct fullDataTurn *bot2Choice(const struct dataBoard *input_board, char side, bool *status) {
	struct standardList *head; // en passant might not work
	struct standardList *head_attacking;
	head = allBasicLegalMoves(input_board, side);
	head_attacking = getOnlyAttackingMoves(head, input_board);
	struct standardList *random_move;
	if (head_attacking != NULL) {
		random_move = getElementOfStandardList(head_attacking, randomInt(0, lengthOfStandardList(head_attacking)) - 1);
	}
	else {
		random_move = getElementOfStandardList(head, randomInt(0, lengthOfStandardList(head)) - 1);
	}
	if (random_move == NULL) {
		random_move = getElementOfStandardList(head, randomInt(0, lengthOfStandardList(head)) - 1);
	}
	if (random_move == NULL) {
		random_move = head; // this feels wrong but idk why it fails otherwise
	}
	*status = 1;
	if (random_move == NULL) {
		*status = 0;
		printf("error\nquiting\n");
		return NULL;
	}
	struct standardPos start = ((struct basicDataTurn*)random_move->data)->starting_pos;
	struct standardPos end = ((struct basicDataTurn*)random_move->data)->ending_pos;
	struct fullDataTurn *final = malloc(sizeof(struct fullDataTurn));
	final->final_position.row = -1;
	final->final_position.col = -1;
	final->starting_position.row = -1;
	final->starting_position.col = -1;
	final->final_position = end;
	final->starting_position = start;
	final->piece = pieceIdOfDataPiece(getDataPiece(input_board, start.row, start.col));
	final->is_en_passant = false; 

	final->piece_promotes_to = 'Q';
	// final->takes = doesTake(input_board, &final->starting_position, &final->final_position);
	final->takes = true;
	//check checking (pun intended)
	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));
	memcpy(copy_board, input_board, sizeof(struct dataBoard));
	copy_board->board[final->starting_position.row][final->starting_position.col] = 31;
	copy_board->board[final->final_position.row][final->final_position.col] = input_board->board[final->starting_position.row][final->starting_position.col];
	final->is_check = kingInCheck(copy_board, oppositeSide(side));
	free(copy_board);

	//these are just assumptions for now
	final->castles = false;
	final->is_king_side = false;
	final->is_special = false;
	//prevents illegal castling
	if (final->piece == 'K' || final->piece == 'R') {
		final->is_special = true;
	}
	//prep for en passant
	if (final->piece == 'P') {
		if (side == 'W' && pieceIdOfDataPiece(getDataPiece(input_board, final->final_position.row, final->final_position.col) == 'P')) {
			if (final->final_position.row == 3 && final->starting_position.row == 1) {
				final->is_special = true;
			}
		}
		if (side == 'B' && pieceIdOfDataPiece(getDataPiece(input_board, final->final_position.row, final->final_position.col) == 'P')) {
			if (final->final_position.row == 4 && final->starting_position.row == 6) {
				final->is_special = true;
			}
		}
	}
	
	return final;
}
