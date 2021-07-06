#include "random-bot.h"


int random_int(int min, int max) { // stolen from stack overflow user= sleutheye
   return min + rand() % (max+1 - min);
}

struct basicDataTurnNode* allBasicLegalMoves(const struct dataBoard* input_board, char side)  {
	struct basicDataTurnNode* head = malloc(sizeof(struct basicDataTurnNode));
	head->next = NULL;
	struct standard_pos temp_start_pos = {.row = 0, .col = 0};
	struct standard_pos temp_end_pos = {.row = 0, .col = 0};
	struct boardCheck* lolm;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (sideOfDataPiece(getDataPiece(input_board, i , j)) == side) {
				temp_start_pos.row = i;
				temp_start_pos.col = j;


				lolm = listOfLegalMoves(input_board, &temp_start_pos, input_board, false);
				if (lolm != NULL) {
					// printBoardCheck(lolm);
					for (int k = 0; k < 8; k++) {
						for (int l = 0; l < 8; l++) {
							if (getBitOfBoardCheck(lolm, positionToIndex(k, l))) {
								temp_end_pos.row = k;
								temp_end_pos.col = l;

								head = appendBasicDataTurn(head, &temp_start_pos, &temp_end_pos);
							}
						}
					}
				}
			}
		}
	}
	return head->next;
}

struct fullDataTurn* randomChoice(const struct dataBoard* input_board, char side, bool* status) {
	struct basicDataTurnNode* head; // en passant might not work
	head = allBasicLegalMoves(input_board, side);
	printf("number of legal moves: %d\n", lengthOfBasicDataTurn(head));
	head = getElementOfBasicDataTurn(head, random_int(0, lengthOfBasicDataTurn(head)));
	*status = 1;
	if (head == NULL) {
		*status = 0;
		return NULL;
	}
	struct standard_pos start = head->starting_pos;
	struct standard_pos end =  head->ending_pos;
	
	struct fullDataTurn* final = malloc(sizeof(struct fullDataTurn));

	final->final_position = end;
	final->starting_position = start;
	final->piece = pieceIdOfDataPiece(getDataPiece(input_board, start.row, start.col));
	final->is_en_passant = false; 
	final->piece_promotes_to = 'Q';
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

	final->takes = (sideOfDataPiece(getDataPiece(input_board, end.row, end.col)) == oppositeSide(side));
	//these are just assumptions for now
	final->castles = false;
	final->is_king_side = false;
	//prevents illegal castling
	if (final->piece == 'K' || final->piece == 'R') {
		final->is_special = true;
	}
	//prep for en passant
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
	return final;
}
