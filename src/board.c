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

bool getBitOfBoardCheck(struct boardCheck* input_mask, unsigned char index) {
	return !!(((uint64_t)1 << index) & input_mask->mask);
}

uint64_t setBitOfBoardCheck(struct boardCheck* input_mask, unsigned char index) {
	return (((uint64_t)1 << index) | input_mask->mask);
}

unsigned char positionToIndex( char row,  char col) {
	return ((row * 8) + col);
}


void printBoardCheck(struct boardCheck* input_mask) {
	for (int i = 8; i > 0; i--) {
		printf("%d  ", i);
		for (int j = 0; j < 8; j++) {
			printf("%d ",  getBitOfBoardCheck(input_mask, positionToIndex(i, j)));
		}
		printf("\n");
	}
	printf("\n");
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
