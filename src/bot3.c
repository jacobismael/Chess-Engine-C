//only attacking when it svaluable and castles when possible
#include "bot3.h"


struct basicDataTurnNode* getOnlyGoodAttackingMoves(struct basicDataTurnNode* head, const struct dataBoard* input_board) {
	struct basicDataTurnNode* new_list_head = malloc(sizeof(struct basicDataTurnNode));
	new_list_head->next = NULL;

	while (head != NULL) {
		if (doesTake(input_board, &head->starting_pos, &head->ending_pos)) {
			//this should use enums instead i ythink 
			if (valueOfDataPiece(getDataPiece(input_board, head->starting_pos.row, head->starting_pos.col)) < valueOfDataPiece(getDataPiece(input_board, head->ending_pos.row, head->ending_pos.col))) {
				new_list_head = appendBasicDataTurn(new_list_head, &head->starting_pos, &head->ending_pos);
			}
		}
		head = head->next;
	}
	struct basicDataTurnNode* result = new_list_head->next;
	free(new_list_head);
	return result;
}

bool canCastle(const struct dataBoard* input_board, char side, bool is_king_side) {
	//it prefers to castle king side
	unsigned char end_row = side == 'W' ? 0 : 7;
	//i thcecks for king side first
	if (kingInCheck(input_board, side)) {
		return false; // you cant castle when in check
	}
	if (is_king_side) {
		if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 5)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 6)) == ' ') { // checks squares are empty
			if (getDataPiece(input_board, end_row, 4) == makeDataPiece('K', side, false) && getDataPiece(input_board, end_row, 7) == makeDataPiece('R', side, false)) {
					struct standard_pos temp_positions[2] = {{.row = end_row, .col = 5}, {.row = end_row, .col = 6}};
					if (!positionUnderAttack(input_board, oppositeSide(side), &temp_positions[0]) && positionUnderAttack(input_board, oppositeSide(side), &temp_positions[1])) {
						return true;
					}
			}
		}
		return false;
	}
	else {
		if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 1)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 2)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 3)) == ' ') { // checks squares are empty
			if (getDataPiece(input_board, end_row, 0) == makeDataPiece('R', side, false) && getDataPiece(input_board, end_row, 4) == makeDataPiece('K', side, false)) {
					struct standard_pos temp_positions[2] = {{.row = end_row, .col = 2}, {.row = end_row, .col = 3}};
					if (!positionUnderAttack(input_board, oppositeSide(side), &temp_positions[0]) && positionUnderAttack(input_board, oppositeSide(side), &temp_positions[1])) {
						return true;
					}
			}
		}
		return false;
	}
}

struct fullDataTurn* bot3Choice(const struct dataBoard* input_board, char side, bool* status) {
	struct basicDataTurnNode* head; // en passant might not work
	struct basicDataTurnNode* head_attacking;
	head = allBasicLegalMoves(input_board, side);
	head_attacking = getOnlyGoodAttackingMoves(head, input_board);
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
	if (canCastle(input_board, side, true)) {
		final->castles = true;
		final->is_king_side = true;
		return final;
	}
	else if (canCastle(input_board, side, true)) {
		final->castles = true;
		final->is_king_side = false;
		return final;
	}
	


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
