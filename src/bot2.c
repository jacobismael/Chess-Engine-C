//random-bot-improved
#include "bot2.h"


struct basicDataTurnNode* getOnlyAttackingMoves(struct basicDataTurnNode* head, const struct dataBoard* input_board) {
	struct basicDataTurnNode* new_list_head = malloc(sizeof(struct basicDataTurnNode));
	new_list_head->next = NULL;

	while (head != NULL) {
		if (doesTake(input_board, &head->starting_pos, &head->ending_pos)) {
			new_list_head = appendBasicDataTurn(new_list_head, &head->starting_pos, &head->ending_pos);
		}
		head = head->next;
	}
	struct basicDataTurnNode* result = new_list_head->next;
	free(new_list_head);
	return result;
}

bool doesTake(const struct dataBoard* input_board, struct standard_pos* starting_position, struct standard_pos* final_position) {
	char side = sideOfDataPiece(getDataPiece(input_board, starting_position->row, starting_position->col));
	if (sideOfDataPiece(getDataPiece(input_board, final_position->row, final_position->col)) == oppositeSide(side)) {
		return true; 
	}
	//includes en passant in taking
	if(pieceIdOfDataPiece(getDataPiece(input_board, starting_position->row, starting_position->col)) == 'P') {
		if (starting_position->col != final_position->col) {
			return true;
		}
	}
	return false;
}

struct fullDataTurn* bot2Choice(const struct dataBoard* input_board, char side, bool* status) {
	struct basicDataTurnNode* head; // en passant might not work
	struct basicDataTurnNode* head_attacking;
	head = allBasicLegalMoves(input_board, side);
	head_attacking = getOnlyAttackingMoves(head, input_board);
	struct basicDataTurnNode* random_move;
	if (head_attacking != NULL) {
		random_move = getElementOfBasicDataTurn(head_attacking, random_int(0, lengthOfBasicDataTurn(head_attacking)));
	}
	else {
		random_move = getElementOfBasicDataTurn(head, random_int(0, lengthOfBasicDataTurn(head)));
	}
	if (random_move == NULL) {
		random_move = getElementOfBasicDataTurn(head, random_int(0, lengthOfBasicDataTurn(head)) - 1);
	}
	if (random_move == NULL) {
		random_move = head; // this feels wrong but idk why it fails otherwise
	}
	*status = 1;
	if (random_move == NULL) {
		*status = 0;
		printf("error\nquiting\n");
		exit(1);
		return NULL;
	}
	struct standard_pos start =  random_move->starting_pos;
	struct standard_pos end =  random_move->ending_pos;
	freeBasicDataTurn(head);
	freeBasicDataTurn(head_attacking);
	
	struct fullDataTurn* final = malloc(sizeof(struct fullDataTurn));
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
	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));
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
