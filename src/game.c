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
	if (attacking_if_taken || TeamOnSquare(input_board, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) { // this
		validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
		
	}
	if (attacking_if_taken || TeamOnSquare(input_board, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) { // this
		validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
		
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
                if(attacking_if_taken || strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL) {

                    validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
                    if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) != ' ') {
                        break;
                    }
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
                if (strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
                    validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
                    if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) != ' ') {
                        break;
                    }
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
                if (strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
                    validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
                    if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) != ' ') {
                        break;
                    }
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
    printf("result: %ld\n", validPositions->mask);
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
	//white's move
	char white_move[6];
	strncpy(white_move, move->white_notation, 6);
	
	input_board = buildFromHalfMove(input_board, white_move, 'W', NULL);
	
	char black_move[6];
	strncpy(black_move, move->black_notation, 6);
	input_board = buildFromHalfMove(input_board, black_move, 'B', NULL);


	return input_board;
}

struct dataBoard* buildFromHalfMove(struct dataBoard* input_board, char* move, char side, int* status) {
	
	*status = 0;
	
	struct dataTurn* cmove = toDataTurn(move);
	if (cmove == NULL) {
		printf("Illegal move sent\n");
		return input_board;
	}
	//castle handling
	if (cmove->castles ==  1) {
		int end_row = side == 'W' ? 0 : 7;
		struct dataPiece blank_piece = makeDataPiece(' ', ' ');
		struct dataPiece king_piece = makeDataPiece('K', side);
		struct dataPiece rook_piece = makeDataPiece('R', side);
		if (cmove->is_king_side) {
			if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 5)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 6)) == ' ') { // checks that the squares between the king are empty
				// printf("here\n");
				if(pieceIdOfDataPiece(getDataPiece(input_board, end_row, 4)) == 'K' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 7)) == 'R') {
					*status = 1;
					
					input_board->board[end_row][4] = blank_piece;
					input_board->board[end_row][5] = rook_piece;
					input_board->board[end_row][6] = king_piece;
					input_board->board[end_row][7] = blank_piece;
				}
			}
		}
		else {
			if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 1)) == ' ' &&  pieceIdOfDataPiece(getDataPiece(input_board, end_row, 2)) == ' '  && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 3)) == ' ') { // checks that the squares between the king are empty
				// printf("here\n");
				if(pieceIdOfDataPiece(getDataPiece(input_board, end_row, 4)) == 'K' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 0)) == 'R') {
					*status = 1;
					
					input_board->board[end_row][0] = blank_piece;
					input_board->board[end_row][1] = blank_piece;
					input_board->board[end_row][2] = king_piece;
					input_board->board[end_row][3] = rook_piece;
					input_board->board[end_row][4] = blank_piece;
				}
			}
		}
		free(cmove);
		return input_board;
	}

	struct dataBoard* original_board = malloc(sizeof(struct dataBoard));
	memcpy(original_board, input_board, sizeof(struct dataBoard)); //this feels inefficient instead maybe save specific positions and 
	
	if (cmove->takes && sideOfDataPiece(getDataPiece(input_board, cmove->final_position.row -1, cmove->final_position.col)) == oppositeSide(side)) {
		if (strchr("K ",  pieceIdOfDataPiece(getDataPiece(input_board, cmove->final_position.row -1, cmove->final_position.col)) == NULL )) { // adds replaced peice to list of pieces that are off the board
			// input_board->off_the_board[0] = input_board->board[cmove->final_position.row][cmove->final_position.col];
			input_board->board[cmove->final_position.row -1 ][cmove->final_position.col].id = 15;
		}

	}

	struct standard_pos temp_position = {.row = 0, .col = 0};
	struct boardCheck* lolm = NULL;
	struct pos expected_result = {.row = cmove->final_position.row -1, .col = cmove->final_position.col, .next=NULL};
	struct standard_pos restrictors = cmove->restrictors;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((restrictors.row == i || restrictors.row == -1) && (restrictors.col == j || restrictors.col == -1)) { 
			// this is The line to focus on
			//also add a game.c with a game struct etc 
				if (pieceIdOfDataPiece(getDataPiece(input_board, i, j)) == cmove->piece && sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
					temp_position.row = i;
					temp_position.col = j; 

					lolm = listOfLegalMoves(input_board, &temp_position, original_board, false);
                    // printf("lolm: %ld\n", lolm->mask);

					if (getBitOfBoardCheck(lolm, positionToIndex(expected_result.row, expected_result.col))) {
                        printBoardCheck(lolm);
						input_board->board[i][j].id = 15;
						*status = 1;
						break;
					}
				}
			}
		}
	}
	if(*status == 1) {
		struct dataPiece temp_piece = makeDataPiece(cmove->piece, side);
		input_board->board[cmove->final_position.row - 1][cmove->final_position.col] = temp_piece; // the -1 is required because arrs start at 0

		if (kingInCheck(input_board, (side))) {
			*status = 0;
			printf("illegal\n");
		}

	}
	free(cmove);
	free(original_board);
	return input_board;
}

bool positionUnderAttack(const struct dataBoard* input_board, char attacking_side, const struct standard_pos* position) { // this is probably broken
    assert(attacking_side == 'W' || attacking_side == 'B');

    struct boardCheck* lolm = malloc(sizeof(struct boardCheck));
	
	struct dataBoard* pawn_board = malloc(sizeof(struct dataBoard));
	memcpy(pawn_board, input_board, sizeof(struct dataBoard));
	if (validRange(position->row) && validRange(position->col)) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (sideOfDataPiece(getDataPiece(input_board, i, j)) == attacking_side) {
					pawn_board->board[position->row][position->col] = makeDataPiece('K', oppositeSide(attacking_side));
					struct standard_pos temp_pos = {.row = i, .col = j};
					lolm->mask |= listOfLegalMoves(input_board, &temp_pos, pawn_board, true)->mask;
				
				}	
			}	
		}
	}
    printBoardCheck(lolm);
    if (getBitOfBoardCheck(lolm, positionToIndex(position->row, position->col))) {
        
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

