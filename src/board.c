#include "board.h"


struct dataPiece getDataPiece(const struct dataBoard* board, signed char row, signed char col) {
	assert(row >= 0 && row <= 7);
	assert(col >= 0 && col <= 7);

	return board->board[row][col];
}

struct dataPiece* getDataPieceMutable(struct dataBoard* board, signed char row, signed char col) {
	assert(row >= 0 && row <= 7);
	assert(col >= 0 && col <= 7);

	return &board->board[row][col];
}

struct pos* pawnMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side) {
	short pawnDir = side == 'W' ? 1 : -1;
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	if ((position->row == 1 && side == 'W') || (position->row == 6 && side == 'B')) { // for moving forward two steps if on row 2
		if (TeamOnSquare(input_board, position->row + 2*pawnDir, position->col) == ' ') { 
			appendPos(validPositions, position->row + 2*pawnDir, position->col);
		}
		else {
			// printf("test:%c:\n", TeamOnSquare(input_board, position->row + 2*pawnDir, position->col));
		}
	}
	if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col) == ' ') {
		appendPos(validPositions, position->row + 1*pawnDir, position->col);
	}
	if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) { // this
		appendPos(validPositions, position->row + 1*pawnDir, position->col - 1);
		
	}
	if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) { // this
		appendPos(validPositions, position->row + 1*pawnDir, position->col + 1);
		
	}
	return validPositions;
}

struct pos* knightMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side) {
	struct pos* knight_pos_offset_head = malloc(sizeof(struct pos));
	knight_pos_offset_head->row = 2;
	knight_pos_offset_head->col = -1; 
	knight_pos_offset_head->next = NULL;
	appendPos(knight_pos_offset_head, 2,   1);
	appendPos(knight_pos_offset_head, 1,   2);
	appendPos(knight_pos_offset_head, -1,  2);
	appendPos(knight_pos_offset_head, -2,  1);
	appendPos(knight_pos_offset_head, -2, -1);
	appendPos(knight_pos_offset_head, -1,  2);
	appendPos(knight_pos_offset_head, 1,   -2);

	validPositions->next = NULL; // the first one is empty to use as a head and is not returned

	while (knight_pos_offset_head != NULL) {
		int new_row = position->row + knight_pos_offset_head->row;
		int new_col = position->col + knight_pos_offset_head->col;
		if (validRange(new_row)  && validRange(new_col)) {
			char combined[] = { ' ', oppositeSide(side)};

			if (strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, new_row, new_col))) != NULL) {

				appendPos(validPositions, new_row, new_col);
			}
		}
		knight_pos_offset_head = knight_pos_offset_head->next;
	}
	freePosList(knight_pos_offset_head);
	return validPositions;
}

struct pos* bishopMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side) {
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	char combined[] = { ' ', oppositeSide(side)};
	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1]) && strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL) {

				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	
	return validPositions;
}

struct pos* rookMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side) {
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	char combined[] = { ' ', oppositeSide(side)};
	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1]) && strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}

struct pos* queenMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side) {
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	char combined[] = {' ', oppositeSide(side)};
	for (int i = 0; i < 8; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1]) && strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (pieceIdOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) != ' ') {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}


struct pos* kingMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side) {
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	struct standard_pos* position_cpy = malloc(sizeof(struct standard_pos));

	for (int i = 0; i < 8; i++) {
		position_cpy->row = position->row + multipliers[i][0];
		position_cpy->col = position->col + multipliers[i][1];

		if (!positionUnderAttack(input_board, oppositeSide(side), position_cpy)) {
			appendPos(validPositions, position_cpy->row, position_cpy->col + multipliers[i][1]);

		}	
	}

	
	// printPosList(validPositions->next);
	free(position_cpy);

	return validPositions;
}

char sideOfDataPiece(const struct dataPiece dp) {
	
	if (dp.id >= 0) {
		if (dp.id <= 5) {
			return 'W';
		}
		else if (dp.id <= 11) {
			return 'B';
		}
	}
	return ' ';
}

char pieceIdOfDataPiece(const struct dataPiece dp) {
	if (dp.id <= 11) {
		switch (dp.id % 6) {
			case 0:
				return 'P';
			case 1:
				return 'R';
			case 2:
				return 'N';
			case 3:
				return 'B';
			case 4:
				return 'Q';
			case 5:
				return 'K';
			default:
				return ' ';
		}
	}
	return ' ';
}


struct dataPiece makeDataPiece(char pieceId, char side) {
	struct dataPiece result;

	if (side == ' ') {
		result.id = 15;
		return result;
	}
	else if (side == 'W') {
		result.id = 0;
	}
	else if (side == 'B') {
		result.id = 6;
	}

	switch (pieceId) {
		case 'P':
			break;
		case 'R':
			result.id += 1;
			break;
		case 'N':
			result.id += 2;
			break;
		case 'B':
			result.id += 3;
			break;
		case 'Q':
			result.id += 4;
			break;
		case 'K':
			result.id += 5;
			break;
		default:
			result.id = 15;
	}

	return result;
}

struct dataPiece pieceToDataPiece(struct piece* p) {
	struct dataPiece result;
	if (p->side == ' ') {
		result.id = 15;
		return result;
	}
	else if (p->side == 'W') {
		result.id = 0;
	}
	else if (p->side == 'B') {
		result.id = 6;
	}

	switch ( p->pieceId) {
		case 'P':
			break;
		case 'R':
			result.id += 1;
			break;
		case 'N':
			result.id += 2;
			break;
		case 'B':
			result.id += 3;
			break;
		case 'Q':
			result.id += 4;
			break;
		case 'K':
			result.id += 5;
			break;
		default:
			result.id = 15;
	}

	return result;
	
}


void printBoard(const struct board* input_board) {
	printf("\n\n\n");
	for (int i = 7; i >= 0; i--) {
		printf("%d  ", i+ 1);
		for (int j = 0; j < 8; j++) {
			printf("%c%c", input_board->board[i][j].side, input_board->board[i][j].pieceId);
			if (j != 7) { 
				printf("|");
			}
		}
		if (i != 0) {
			printf("\n   --+--+--+--+--+--+--+--\n");
		}
		else {
			printf("\n");
		}
		
	}
	printf("   a  b  c  d  e  f  g  h\n");
}

void printDataBoard(const struct dataBoard* input_board) {

	printf("\n\n\n");
	for (int i = 7; i >= 0; i--) {
		printf("%d  ", i + 1);
		for (int j = 0; j < 8; j++) {
			
			printf("%c%c", sideOfDataPiece(getDataPiece(input_board, i, j)), pieceIdOfDataPiece(getDataPiece(input_board, i, j)));

			if (j != 7) { 
				printf("|");
			}
		}
		if (i != 0) {
			printf("\n   --+--+--+--+--+--+--+--\n");
		}
		else {
			printf("\n");
		}
		
	}
	printf("   a  b  c  d  e  f  g  h\n");
}


struct board* setupBoard() {
	char* side_template = "RNBQKBNR";
	struct board* newBoard = malloc(sizeof(struct board));
	//fills in the board with the standard configuration
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			newBoard->board[row][col].side = row <= 1 ? 'W' : (row >= 6 ? 'B' : ' ');
			if (row == 0 || row == 7) { 
			        newBoard->board[row][col].pieceId = side_template[col];
			}
			else if (row == 1 || row == 6) {
				newBoard->board[row][col].pieceId = 'P';
			}
			else {
			    newBoard->board[row][col].pieceId = ' ';
			}
		}
	}
	//sets the pieces off board to 0
	// for (int i = 0; i < 48; i++) {
	// 	struct piece newPiece = {.pieceId=' ', .side=' '};
	// 	// newBoard->off_the_board[i] = newPiece;
	// }
	return newBoard;
}

struct dataBoard* setupDataBoard() {
	int side_template[8] = {1, 2, 3, 4, 5, 3, 2, 1};
	struct dataBoard* newBoard = malloc(sizeof(struct dataBoard));
	//fills in the board with the standard configuration
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			newBoard->board[row][col].id = row <= 1 ? 0 : (row >= 6 ? 6 : 0);
			if (row == 0 || row == 7) { 
			    newBoard->board[row][col].id += side_template[col];
			}
			else if(row != 6 && row != 1) {
			    newBoard->board[row][col].id = 13;
			}
		}
	}
	return newBoard;
}

bool validRange(int input) {
	if (input >= 0 && input <= 7) {
		return true;
	}
	return false;
}

char TeamOnSquare(const struct dataBoard* input_board, int row, int col) {
	if (!validRange(row) || !validRange(col)) {
		return ' ';
	}
	return sideOfDataPiece(getDataPiece(input_board, row, col));
}

struct pos* listOfLegalMoves(const struct dataBoard* input_board, const struct standard_pos* position, const struct dataBoard* original_board) { //replace original board with a struct called board diff or smth
	struct pos* validPositions = malloc(sizeof(struct pos));
	validPositions->next = NULL;
	char side = sideOfDataPiece(getDataPiece(input_board, position->row, position->col));
	// printf("side: %c\n", side);
	// printf("piece: %c\n", pieceIdOfDataPiece(&input_board->board[position->row][position->col]));
	// printf("position: %d %d\n", position->row, position->col);
	switch(pieceIdOfDataPiece(getDataPiece(input_board, position->row, position->col))) {
		case ' ':
			return NULL;
		case 'P':
			validPositions = pawnMovement(original_board, validPositions, position, side);
			break;
		case 'N':
			validPositions = knightMovement(input_board, validPositions, position, side);
			break;
		case 'B':
			validPositions = bishopMovement(input_board, validPositions, position, side);
			break;
		case 'R':
			validPositions = rookMovement(input_board, validPositions, position, side);
			break;
		case 'Q':
			validPositions = queenMovement(input_board, validPositions, position, side);
			break;
		case 'K':
			validPositions = kingMovement(input_board, validPositions, position, side);
			break;
	}
	struct pos* temp = validPositions->next; // we return next because the first value is just used  to set it up and has no real value
	free(validPositions);
	return temp;
}


bool positionUnderAttack(const struct dataBoard* input_board, char attacking_side, const struct standard_pos* position) { // this is probably broken
	struct pos* lolm = NULL;
	
	struct dataBoard* pawn_board = malloc(sizeof(struct dataBoard));
	memcpy(pawn_board, input_board, sizeof(struct dataBoard));
	// printDataBoard(input_board);
	if (validRange(position->row) && validRange(position->col)) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (sideOfDataPiece(getDataPiece(input_board, i, j)) == attacking_side) {
					printf("peice, %c%c\n", sideOfDataPiece(getDataPiece(input_board, i, j)), pieceIdOfDataPiece(getDataPiece(input_board, i, j)));
					pawn_board->board[position->row][position->col] = makeDataPiece('K', oppositeSide(attacking_side));
					struct standard_pos temp_pos = {.row = i, .col = j};
					lolm = listOfLegalMoves(input_board, &temp_pos, pawn_board);
				
					struct pos expected_pos = standard_posToPos(position);
					if (lolm != NULL && posLlContains(lolm, &expected_pos)) {
						printf("position under attack at %d %d", position->row, position->col);
						free(pawn_board);
						freePosList(lolm);
						return true;
					}
				}	
			}	
		}
	}
	free(pawn_board);
	freePosList(lolm);
	return false;
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

bool kingInCheck(const struct dataBoard* input_board, char side) {
	assert(side == 'W' || side == 'B');
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
				if (pieceIdOfDataPiece(getDataPiece(input_board, i, j)) == 'K') {
					struct standard_pos temp_pos = {.row = i, .col = j};
					printf("king is at %d %d \n", i, j);
					if (positionUnderAttack(input_board, oppositeSide(side), &temp_pos)) {
						return true;
					}
					return false;
				}
			}
		}
	}
	assert(0 == 1);

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
	struct pos* lolm = NULL;
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

					lolm = listOfLegalMoves(input_board, &temp_position, original_board);

					if (posLlContains(lolm, &expected_result)) {
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

		if (kingInCheck(input_board, side)) {
			*status = 0;
			printf("illegal");
		}
		else {
			printDataBoard(input_board);
			printf("yeet\n");

		}
	}
	freePosList(lolm);
	free(cmove);
	free(original_board);
	return input_board;
}

