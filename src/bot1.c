//random-bot
#include "bot1.h"



struct fullDataTurn* bot1Choice(const struct dataBoard* input_board, char side, bool* status) {
	struct basicDataTurnNode* head; // en passant might not work
	head = allBasicLegalMoves(input_board, side);
	// printf("number of legal moves: %d\n", lengthOfBasicDataTurn(head));
	struct basicDataTurnNode* random_move = getElementOfBasicDataTurn(head, random_int(0, lengthOfBasicDataTurn(head)));
	*status = 1;
	if (random_move == NULL) {
		random_move = head;
	}
	if (random_move == NULL) {
		*status = 0;
		return NULL;
	}
	struct standard_pos start =  random_move->starting_pos;
	struct standard_pos end =  random_move->ending_pos;
	freeBasicDataTurn(head);
	
	struct fullDataTurn* final = malloc(sizeof(struct fullDataTurn));

	final->final_position = end;
	final->starting_position = start;
	final->piece = pieceIdOfDataPiece(getDataPiece(input_board, start.row, start.col));
	final->is_en_passant = false; 

	final->piece_promotes_to = 'Q';
	final->takes = (sideOfDataPiece(getDataPiece(input_board, end.row, end.col)) == oppositeSide(side));
	//includes en passant in taking
	if (!final->takes) {
		if(final->piece == 'P') {
			if (final->starting_position.col != final->final_position.col) {
				final->takes = true;
			}
		}
	}
	if (final->takes && sideOfDataPiece(getDataPiece(input_board, final->final_position.row , final->final_position.col)) == ' ') {
		if (isDataPieceSpecial(getDataPiece(input_board, final->final_position.row + (side == 'W' ? -1 : 1), final->final_position.col))) {
			final->is_en_passant = true;
		}
	}
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
