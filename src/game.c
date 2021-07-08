#include "game.h"


struct boardCheck* pawnMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken) {
    short pawnDir = side == 'W' ? 1 : -1;
    if (!attacking_if_taken) {
        if ((position->row == 1 && side == 'W') || (position->row == 6 && side == 'B')) { // for moving forward two steps if on row 2
            if (TeamOnSquare(input_board, position->row + 2*pawnDir, position->col) == ' ') { 
                validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 2*pawnDir, position->col));
            }
            else {
                // printf("test:%c:\n", TeamOnSquare(input_board, position->row + 2*pawnDir, position->col));
            }
        }
        if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col) == ' ') {
            validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col));
        }
    }
	//en passant tstuff
	if (side == 'W' && position->row == 4){
		// printf("piece to the right is of type: %d \n", getDataPiece(input_board, position->row, position->col + 1));
		// printf("piece to the right is of special %d \n", isDataPieceSpecial(getDataPiece(input_board, position->row, position->col + 1)));
		
		if ( validRange(position->col + 1) && getDataPiece(input_board, position->row, position->col + 1) == 13) {
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
		}
		else if(validRange(position->col - 1) && getDataPiece(input_board, position->row, position->col - 1) == 13 ) { // this is else ifbecause there is never more than one en passant possibility at a time
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
		}
	}

	if (side == 'B' && position->row == 3){
		if (validRange(position->col + 1) && getDataPiece(input_board, position->row, position->col + 1) == 12) {
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
		}
		else if(validRange(position->col - 1) && getDataPiece(input_board, position->row, position->col - 1) == 12) { // this is else ifbecause there is never more than one en passant possibility at a time
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
		}
	}


	// normal taking
	if (attacking_if_taken || TeamOnSquare(input_board, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) { // this
		if (validRange(position->row + 1*pawnDir) && validRange(position->col - 1)) {
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
		}
	}
	if (attacking_if_taken || TeamOnSquare(input_board, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) { // this
		if (validRange(position->row + 1*pawnDir) && validRange(position->col + 1)) {
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
		}
	}
	return validPositions;
}

struct boardCheck* knightMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken) {

    int multipliers[8][2] = {
    {2,  -1}, 
    {2,   1},
    {1,   2},
    {-1, -2},
    {-2,  1},
    {-2, -1},
    {-1,  2},
    {1,  -2}
    };

	for (int i = 0; i < 8; i++) {
		int new_row = position->row + multipliers[i][0];
		int new_col = position->col + multipliers[i][1];
		if (validRange(new_row)  && validRange(new_col)) {
			char combined[] = { ' ', oppositeSide(side)};

			if (attacking_if_taken || strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, new_row, new_col))) != NULL) {

				validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(new_row, new_col));
			}
		}
	}
	
	return validPositions;
}

struct boardCheck* bishopMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken) {
	int multipliers[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	char combined[] = { ' ', oppositeSide(side)};
	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1])) {
               if (strchr(combined, sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
					validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
					if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {

						break;	
					}
                }
				else if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == side) {

					break;
				}
			}
		}
	}
	
	return validPositions;
}

struct boardCheck* rookMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken) {
	int multipliers[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	char combined[] = { ' ', oppositeSide(side)};
	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1])) {
                if (strchr(combined, sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
					validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
					if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {

						break;	
					}
                }
				else if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == side) {

					break;
				}
            }
		}
	}
	// printPosList(validPositions);
	return validPositions;
}

struct boardCheck* queenMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken) {
	
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	char combined[] = {' ', oppositeSide(side)};
	for (int i = 0; i < 8; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1])) {
                if (strchr(combined, sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
					validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
					if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {

						break;	
					}
                }
				else if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == side) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}


struct boardCheck* kingMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken) {
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	struct standard_pos* position_cpy = malloc(sizeof(struct standard_pos));

	for (int i = 0; i < 8; i++) {
		position_cpy->row = position->row + multipliers[i][0];
		position_cpy->col = position->col + multipliers[i][1];
        if (validRange(position_cpy->row) && validRange(position_cpy->col)) {
            if (attacking_if_taken || sideOfDataPiece(getDataPiece(input_board, position_cpy->row, position_cpy->col)) != side) {
                validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + multipliers[i][0], position->col + multipliers[i][1]));
            }	
        }
	}
	// printf("king\n");
	// printBoardCheck(validPositions);

	
	// printPosList(validPositions->next);
	free(position_cpy);

	return validPositions;
}

struct boardCheck* listOfLegalMoves(const struct dataBoard* input_board, const struct standard_pos* position, const struct dataBoard* original_board, bool attacking_self) { //replace original board with a struct called board diff or smth
	struct boardCheck* validPositions = malloc(sizeof(struct boardCheck));
    validPositions->mask = 0;
	char side = sideOfDataPiece(getDataPiece(input_board, position->row, position->col));
	// printf("side: %c\n", side);
	// printf("piece: %c\n", pieceIdOfDataPiece(&input_board->board[position->row][position->col]));
	// printf("position: %d %d\n", position->row, position->col);
	switch(pieceIdOfDataPiece(getDataPiece(input_board, position->row, position->col))) {
		case ' ':
			return NULL;
		case 'P':
			validPositions = pawnMovement(original_board, validPositions, position, side, attacking_self);
			break;
		case 'N':
			validPositions = knightMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'B':
			validPositions = bishopMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'R':
			validPositions = rookMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'Q':
			validPositions = queenMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'K':
			validPositions = kingMovement(input_board, validPositions, position, side, attacking_self);
			break;
	}
    // printf("result: %ld\n", validPositions->mask);
	return validPositions;
}

struct dataBoard* buildFromStart(struct dataBoard* input_board, struct Move* head) {
	while (head != NULL) {
		input_board = buildFromMove(input_board, head);
		printDataBoard(input_board);
		
		head = head->next;
	}
	return input_board;
}


struct dataBoard* buildFromMove(struct dataBoard* input_board, struct Move* move) {
	/*
	//white's move
	char white_move[6];
	strncpy(white_move, move->white_notation, 6);
	
	input_board = buildFromHalfMove(input_board, white_move, 'W', NULL);
	
	char black_move[6];
	strncpy(black_move, move->black_notation, 6);
	input_board = buildFromHalfMove(input_board, black_move, 'B', NULL);

	*/
	return input_board;
}

struct dataBoard* castleHandling(struct dataBoard* input_board, struct fullDataTurn* truemove, char side, bool* status) {
	*status = false;
    if (kingInCheck(input_board, side)) {
        return input_board;
    }
    int end_row = side == 'W' ? 0 : 7;
    unsigned char blank_piece = makeDataPiece(' ', ' ', false);
    unsigned char king_piece = makeDataPiece('K', side, true);
    unsigned char rook_piece = makeDataPiece('R', side, true);
    if (truemove->is_king_side) {
        if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 5)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 6)) == ' ') { // checks that the squares between the king are empty
            
            struct standard_pos positions_to_eval[2] = {
                {.row = end_row, .col = 5},
                {.row = end_row, .col = 6}    
            };
            // we dont need to check the king's square because then it would be in check and we have already checked for that
            if (!positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[0])) && !positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[1]))) { 
                if(pieceIdOfDataPiece(getDataPiece(input_board, end_row, 4)) == 'K' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 7)) == 'R') {
					if (!isDataPieceSpecial(getDataPiece(input_board, end_row, 4)) && !isDataPieceSpecial(getDataPiece(input_board, end_row, 7))) {
						*status = true;
						
						input_board->board[end_row][4] = blank_piece;
						input_board->board[end_row][5] = rook_piece;
						input_board->board[end_row][6] = king_piece;
						input_board->board[end_row][7] = blank_piece;
					}
                }
            }
        }
    }
    else {
        if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 1)) == ' ' &&  pieceIdOfDataPiece(getDataPiece(input_board, end_row, 2)) == ' '  && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 3)) == ' ') { // checks that the squares between the king are empty
            // printf("here\n");
            struct standard_pos positions_to_eval[2] = {
                {.row = end_row, .col = 2},
                {.row = end_row, .col = 3}    
            };
            if (!positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[0])) && !positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[1]))) { 
                if(pieceIdOfDataPiece(getDataPiece(input_board, end_row, 4)) == 'K' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 0)) == 'R') {
					if (!isDataPieceSpecial(getDataPiece(input_board, end_row, 4)) && !isDataPieceSpecial(getDataPiece(input_board, end_row, 0))) {
						*status = true;
						
						input_board->board[end_row][0] = blank_piece;
						input_board->board[end_row][1] = blank_piece;
						input_board->board[end_row][2] = king_piece;
						input_board->board[end_row][3] = rook_piece;
						input_board->board[end_row][4] = blank_piece;
					}
                }
            }
        }
    }
    free(truemove);
    return input_board;
}

struct dataBoard* removeEnPassants(struct dataBoard* input_board, char side) {
	
	for (int i = 0; i < 8; i++ ) {
		for (int j = 0; j < 8; j++ ) {
			if (side == 'W' && input_board->board[i][j] == 12) {
				input_board->board[i][j] = 0;
			}
			else if (side == 'B' && input_board->board[i][j] == 13) {
				input_board->board[i][j] = 6;
			}
		}	
	}

	return input_board;
}

struct fullDataTurn* toFullDataTurn(struct dataTurn* input_turn, struct dataBoard* input_board, char side, bool *status) {
	struct fullDataTurn* final = malloc(sizeof(struct fullDataTurn));
	final->is_special = false;
	*status = 0;
	final->castles = input_turn->castles;
	final->is_king_side = input_turn->is_king_side;
	final->is_check = input_turn->is_check;
	if (final->castles) {
		*status = 1;
		return final;
	}

	final->final_position.row = input_turn->final_position.row - 1, 
	final->final_position.col = input_turn->final_position.col;
	assert(validRange(final->final_position.row));
	assert(validRange(final->final_position.col));
	final->piece_promotes_to = input_turn->piece_promotes_to;
	if(final->piece_promotes_to == 'K') {// you cannot promote to a king
		*status = 0;
		printf("you cannot promot to a king");
		return final;

	}
	final->piece = input_turn->piece;
	final->takes = input_turn->takes;


	//en passant
	final->is_en_passant = false; 
	if (final->takes && sideOfDataPiece(getDataPiece(input_board, final->final_position.row , final->final_position.col)) == ' ') {
		if (isDataPieceSpecial(getDataPiece(input_board, final->final_position.row + (side == 'W' ? -1 : 1), final->final_position.col))) {
			final->is_en_passant = true;
		}
	}


	bool is_piece_special = false;
	assert(side == 'W' || side == 'B');
	
	

	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));
	memcpy(copy_board, input_board, sizeof(struct dataBoard)); //this feels inefficient instead maybe save specific positions and 

	
	if (input_turn->takes && validRange(input_turn->final_position.row - 1)) {
		if (sideOfDataPiece(getDataPiece(copy_board, input_turn->final_position.row -1, input_turn->final_position.col)) == oppositeSide(side)) {
			if (strchr("K ",  pieceIdOfDataPiece(getDataPiece(copy_board, input_turn->final_position.row -1, input_turn->final_position.col)) == NULL )) { // adds replaced peice to list of pieces that are off the board
				// input_board->off_the_board[0] = input_board->board[cmove->final_position.row][cmove->final_position.col];
				copy_board->board[input_turn->final_position.row -1 ][input_turn->final_position.col] = 31;
			}
		}
	}

	struct standard_pos temp_position = {.row = 0, .col = 0};
	struct boardCheck* lolm = NULL;
	struct pos expected_result = {.row = input_turn->final_position.row -1, .col = input_turn->final_position.col, .next=NULL};
	struct standard_pos restrictors = input_turn->restrictors;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((restrictors.row == i || restrictors.row == -1) && (restrictors.col == j || restrictors.col == -1)) { 
			// this is The line to focus on
			//also add a game.c with a game struct etc 
				if (pieceIdOfDataPiece(getDataPiece(copy_board, i, j)) == input_turn->piece && sideOfDataPiece(getDataPiece(copy_board, i, j)) == side) {
					temp_position.row = i;
					temp_position.col = j; 

					lolm = listOfLegalMoves(copy_board, &temp_position, input_board, false);
                    // printf("lolm: %ld\n", lolm->mask);

					if (getBitOfBoardCheck(lolm, positionToIndex(expected_result.row, expected_result.col))) {
                        // printBoardCheck(lolm);
						//checks if piece can be en passanted in the future
						if (!is_piece_special) {
							is_piece_special = isDataPieceSpecial(getDataPiece(copy_board, i, j));
						}
						if (!is_piece_special && input_turn->piece == 'P' ) {
							if(side == 'W') {
								if (expected_result.row == 3 && i == 1) {
									is_piece_special = true;
								}
							}
							else if(side == 'B') {
								if (expected_result.row == 4 && i == 6) {
									is_piece_special = true;
								}
							}
						}
						copy_board->board[i][j] = 31;
						*status = 1;
						final->starting_position = temp_position;
                        free(lolm);
						break;
					}
                    free(lolm);
				}
			}
		}
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

	if (final->piece == 'K' || final->piece == 'R') {
		final->is_special = true;
	}

	// assert(status == 1);

	free(copy_board);
	return final;
}

bool isMate(struct dataBoard* input_board, char side) {
	//this doesnt support en passant yet
	if (!kingInCheck(input_board, side)) {
		return false;
	}

	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));

	unsigned char temp_piece = ' ';
	struct standard_pos temp_pos; 
	struct boardCheck* lolm = NULL;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (validRange(i/* is this right? */) &&  sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
				temp_pos.row = i;
				temp_pos.col = j;
				
				lolm = listOfLegalMoves(input_board, &temp_pos, copy_board, false);
				// printf("piece: %c\n", pieceIdOfDataPiece(getDataPiece(input_board, temp_pos.row, temp_pos.col)));
				// printBoardCheck(lolm);
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						memcpy(copy_board, input_board, sizeof(struct dataBoard));
						if(getBitOfBoardCheck(lolm, positionToIndex(k, l))) {
							temp_piece = copy_board->board[temp_pos.row][temp_pos.col];
							copy_board->board[temp_pos.row][temp_pos.col] = 31;
							copy_board->board[k][l] = temp_piece;
							// printDataBoard(copy_board);

							if (!kingInCheck(copy_board, side)) {
								free(copy_board);
								return false;
							}
						}
					}
				}
				free(lolm);
			}
		}
	}
	free(copy_board);
	return true;

}

int random_int(int min, int max) { // stolen from stack overflow user= sleutheye
   return min + rand() % (max+1 - min);
}

struct basicDataTurnNode* allBasicLegalMoves(const struct dataBoard* input_board, char side)  {
	struct basicDataTurnNode* head = malloc(sizeof(struct basicDataTurnNode));
	head->next = NULL;
	struct standard_pos temp_start_pos = {.row = 0, .col = 0};
	struct standard_pos temp_end_pos = {.row = 0, .col = 0};
	struct boardCheck* lolm;
	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));
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
								memcpy(copy_board, input_board, sizeof(struct dataBoard));
								temp_end_pos.row = k;
								temp_end_pos.col = l;
								
								copy_board->board[i][j] = 31;
								copy_board->board[k][l] = input_board->board[i][j];

								if (!kingInCheck(copy_board, side)) {
									head = appendBasicDataTurn(head, &temp_start_pos, &temp_end_pos);
								}
							}
						}
					}
					free(lolm);
				}
			}
		}
	}
	free(copy_board);
	struct basicDataTurnNode* result = head->next;
	free(head);
	
	return result;
}


bool isDraw(struct dataBoard* input_board, char side) { // very simple and incomplete still
	//this whole function doesnt support en passant yet
	if (kingInCheck(input_board, side)) {
		return false;
	}
	struct basicDataTurnNode* head;
	head = allBasicLegalMoves(input_board, side);
	if (head == NULL) {
		return true;
	}
	
	// 0 1 2 3 4 5
	// K Q B N R P
	int num_each_piece_w[6] = {0};
	int num_each_piece_b[6] = {0};
	int position = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (pieceIdOfDataPiece(getDataPiece(input_board, i, j))) {
				case 'K':
					position = 0;
					break;
				case 'Q':
					position = 1;
					break;
				case 'B':
					position = 2;
					break;
				case 'N':
					position = 3;
					break;
				case 'R':
					position = 4;
					break;
				case 'P':
					position = 5;
					break;					
			}
			if (sideOfDataPiece(getDataPiece(input_board, i, j)) == 'W') {
				num_each_piece_w[position]++;
			}
			else if (sideOfDataPiece(getDataPiece(input_board, i, j)) == 'B') {
				num_each_piece_b[position]++;
			}
		}	
	}
	//cases where it isnt a draw
	if (num_each_piece_b[5] + num_each_piece_w[5] >= 1) { //any pawns left
		return false;
	}
	if (num_each_piece_b[1] + num_each_piece_w[1] >= 1) { //any pawns left
		return false;
	}
	if ((num_each_piece_b[2] >= 1 && num_each_piece_b[3] >= 1) && (num_each_piece_w[2] >= 1 && num_each_piece_w[3] >= 1)) {
		return false;
	}
	if (num_each_piece_b[4] + num_each_piece_w[4] >= 1) {
		return false;
	}
	return true;

}


struct fullDataTurn* stringToFullDataTurn(struct dataBoard* input_board, char* turn, char side, bool* status) {
    	struct dataTurn* cmove = toDataTurn(turn);

	if (cmove == NULL) {
		printf("Illegal move sent\n");
		*status = 0;
		return NULL;
	}

	struct fullDataTurn* truemove = toFullDataTurn(cmove, input_board, side, status);
	free(cmove);
	return truemove;
}

struct dataBoard* buildFromHalfMove(struct dataBoard* input_board, struct fullDataTurn* truemove, char side, bool* status) {
	if (!*status) {
		return input_board;
	}
	struct dataBoard* copy_board = malloc(sizeof(struct dataBoard));
	memcpy(copy_board, input_board, sizeof(struct dataBoard));
	//castle handling
	if (truemove->castles ==  1) {
		free(copy_board);
		printf("in castles\n");
		return castleHandling(input_board, truemove, side, status);
    	}
	//remove peices marked as being able to be enpassanted with
	input_board = removeEnPassants(input_board, side); // remove en passants for own side from previous move
	unsigned char piece_cpy = input_board->board[truemove->starting_position.row][truemove->starting_position.col];
	if (*status == 1) {
		input_board->board[truemove->starting_position.row][truemove->starting_position.col] = 31;
	}
	//assigns special pieces
	if (*status == 1) {
		if (truemove->is_special) {
			input_board->board[truemove->final_position.row][truemove->final_position.col] = makeDataPiece(truemove->piece, side, truemove->is_special);
		} 
		else {
			input_board->board[truemove->final_position.row][truemove->final_position.col] = piece_cpy;
		}
	}
	//en passant remove piece that was taken
	if (truemove->is_en_passant) {
		input_board->board[truemove->final_position.row + (side == 'W' ? -1 : 1)][truemove->final_position.col] = 31;
	}
	//promotion
	if (truemove->piece =='P') {
		printf("promotion1: %c\n", truemove->piece_promotes_to);
		if (truemove->final_position.row == 7 && side == 'W') {
			// it defaults to queen if nothing is specified
			input_board->board[truemove->final_position.row][truemove->final_position.col] = (truemove->piece_promotes_to != ' ' ? makeDataPiece(truemove->piece_promotes_to, side, false) : makeDataPiece('Q', side, false));
		}
		if (truemove->final_position.row == 0 && side == 'B') {
			// it defaults to queen if nothing is specified
			input_board->board[truemove->final_position.row][truemove->final_position.col] = (truemove->piece_promotes_to != ' ' ? makeDataPiece(truemove->piece_promotes_to, side, false) : makeDataPiece('Q', side, false));
		}
	}

	if (kingInCheck(input_board, side)) {
		*status = 0;
	}
	if (*status == 0) {
		memcpy(input_board, copy_board, sizeof(struct dataBoard));
	}
	free(truemove);
	free(copy_board);
	return input_board;
}

bool positionUnderAttack(const struct dataBoard* input_board, char attacking_side, const struct standard_pos* position) { // this is probably broken
    assert(attacking_side == 'W' || attacking_side == 'B');

    struct boardCheck* lolm = malloc(sizeof(struct boardCheck));
    lolm->mask = 0;
    
	struct boardCheck* temp = malloc(sizeof(struct boardCheck));
    free(temp);
	struct dataBoard* pawn_board = malloc(sizeof(struct dataBoard));

	memcpy(pawn_board, input_board, sizeof(struct dataBoard));
	if (validRange(position->row) && validRange(position->col)) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (sideOfDataPiece(getDataPiece(input_board, i, j)) == attacking_side) {
					pawn_board->board[position->row][position->col] = makeDataPiece('K', oppositeSide(attacking_side), true); // the last bool can be true or false in this situation 
					struct standard_pos temp_pos = {.row = i, .col = j};
					temp = listOfLegalMoves(input_board, &temp_pos, pawn_board, true);
                    lolm->mask |= temp->mask;
                    free(temp);
				}	
			}	
		}
	}
    // printBoardCheck(lolm);
    if (getBitOfBoardCheck(lolm, positionToIndex(position->row, position->col))) {
        free(lolm);   
        free(pawn_board);
        return true;
    }
    free(lolm);
	free(pawn_board);
	return false;
} 

bool kingInCheck(const struct dataBoard* input_board, char side) {
	assert(side == 'W' || side == 'B');
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
            if (pieceIdOfDataPiece(getDataPiece(input_board, i, j)) == 'K') {
			    if (sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
                    // printf("king is at %d %d\n", i, j);
					struct standard_pos temp_pos = {.row = i, .col = j};
					bool x =  positionUnderAttack(input_board, oppositeSide(side), &temp_pos);
                    // printf( "is king under attack: %d", x);
                    return x;
                }
			}
		}
	}
	assert(0 == 1);
 
}

