#include "board.h"

unsigned char getDataPiece(const struct dataBoard *board, signed char row, signed char col) {
	assert(row >= 0 && row <= 7);
	assert(col >= 0 && col <= 7);

	return board->board[row][col];
}

unsigned char *getDataPieceMutable(struct dataBoard *board, signed char row, signed char col) {
	assert(row >= 0 && row <= 7);
	assert(col >= 0 && col <= 7);

	return &board->board[row][col];
}

bool isDataPieceSpecial(const unsigned char dp) {
	if (dp >= 12 && dp <= 17) {
		return true;
	}
	return false;
}

char sideOfDataPiece(const unsigned char dp) {
	
	if (dp <= 5) {
		return 'W';
	}
	else if (dp <= 11) {
		return 'B';
	}
	if (dp <= 17) {
		return dp % 2 == 0 ? 'W': 'B';
	}
	
	return ' ';
}

float getBasicBoardScore(const struct dataBoard *input_board) {
	float score = 0;
	float temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (sideOfDataPiece(getDataPiece(input_board, i, j)) == 'W') {
				temp = valueOfDataPiece(getDataPiece(input_board, i, j));
			}
			else {
				temp = -valueOfDataPiece(getDataPiece(input_board, i, j));
			}
			if (temp != INFINITY && temp != -INFINITY) {
				score += temp;
			}
		}
	}

	return score;
}

char pieceIdOfDataPiece(const unsigned char dp) {
	if (dp <= 11) {
		switch (dp % 6) {
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
	else if (dp <= 17) {
		if (dp == 12 || dp == 13) {
			return 'P';
		}
		else if(dp == 14 || dp == 15) {
			return 'K';
		}
		else if(dp == 16 || dp == 17) {
			return 'R';
		}
	}
	return ' ';
}

float valueOfDataPiece(const unsigned char dp) { // float ofr infinity and for future changes to weights of peices?
	if (dp <= 11) {
		switch (dp % 6) {
			case 0:
				return 1;
			case 1:
				return 5;
			case 2:
				return 3;
			case 3:
				return 3;
			case 4:
				return 9;
			case 5:
				return INFINITY;
		}
	}
	else if (dp <= 17) {
		if (dp == 12 || dp == 13) {
			return 1;
		}
		else if(dp == 14 || dp == 15) {
			return INFINITY;
		}
		else if(dp == 16 || dp == 17) {
			return 5;
		}
	}
	return 0;
}

unsigned char makeDataPiece(char pieceId, char side, bool isSpecial) {
	unsigned char result;
	if (!isSpecial) {
		if (side == ' ') {
			result = 31;
			return result;
		}
		else if (side == 'W') {
			result = 0;
		}
		else if (side == 'B') {
			result = 6;
		}

		switch (pieceId) {
			case 'P':
				break;
			case 'R':
				result += 1;
				break;
			case 'N':
				result += 2;
				break;
			case 'B':
				result += 3;
				break;
			case 'Q':
				result += 4;
				break;
			case 'K':
				result += 5;
				break;
			default:
				result =31;
		}
	}
	else {
		assert(side == 'W' || side == 'B');
		assert(pieceId == 'P' || pieceId == 'K' || pieceId == 'R');

		if (pieceId == 'P') {
			result = side == 'W' ? 12 : 13;
		}
		else if (pieceId == 'K') {
			result = side == 'W' ? 14 : 15;
		}
		else if (pieceId == 'R') {
			result = side == 'W' ? 16 : 17;
		}

	}

	return result;
}

bool doesTake(const struct dataBoard *input_board, struct standardPos *starting_position, struct standardPos *final_position) {
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

void printDataBoardDebug(const struct dataBoard *input_board) {

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

void printDataBoard(const struct dataBoard *input_board) {

	printf("\n\n\n");
	for (int i = 7; i >= 0; i--) {
		printf("%d ", i + 1);
		for (int j = 0; j < 8; j++) {
			if (sideOfDataPiece(getDataPiece(input_board, i, j)) == 'W') {
				printf("\033[30;47m%c%c\033[0m", sideOfDataPiece(getDataPiece(input_board, i, j)), pieceIdOfDataPiece(getDataPiece(input_board, i, j)));
			}
			else if (sideOfDataPiece(sideOfDataPiece(getDataPiece(input_board, i, j)) == 'B')){
				printf("\033[37;40m%c%c\033[0m", sideOfDataPiece(getDataPiece(input_board, i, j)), pieceIdOfDataPiece(getDataPiece(input_board, i, j)));
			}
			else { // this doesnt just printf "  " for debugging reasons
				printf("%c%c\033[0m", sideOfDataPiece(getDataPiece(input_board, i, j)), pieceIdOfDataPiece(getDataPiece(input_board, i, j)));
			}

			if (j != 7) { 
				printf("|");
			}
		}
		if (i != 0) {
			printf("\n  --+--+--+--+--+--+--+--\n");
		}
		else {
			printf("\n");
		}
		
	}
	printf("   a  b  c  d  e  f  g  h\n");
}

bool getBitOfBoardCheck(struct boardCheck *input_mask, unsigned char index) {
	return !!(((uint64_t)1 << index) & input_mask->mask);
}

uint64_t setBitOfBoardCheck(struct boardCheck *input_mask, unsigned char index) {
	return (((uint64_t)1 << index) | input_mask->mask);
}

unsigned char positionToIndex( char row,  char col) {
	assert(row <= 8 && row >= 0);
	assert(col <= 8 && col >= 0);
	return ((row  *8) + col);
}

void printBoardCheck(struct boardCheck *input_mask) {
	for (int i = 0; i < 8; i++) {
		printf("%d ", i);
		for (int j = 0; j < 8; j++) {
			printf("%d ", getBitOfBoardCheck(input_mask, positionToIndex(i, j)));
		}
		printf("\n");
	}
	printf("\n");
}

int lengthOfLinkedList(const struct standardList *head) {
	int length = 0;	
	
	while(head != NULL) {
		length++;
		head = head->next;
	}

	return length;
}

void freeLinkedList(struct standardList *head) {
		
	struct standardList *temp_head;
	while(head != NULL) {
		temp_head = head;
        head = head->next;
        free(temp_head);
	}
}



struct standardList *getElementOfLinkedList(struct standardList *head, int position) {	
    if (position > lengthOfLinkedList(head) - 1) {
        return NULL;
    }
    int length = 0;
	
	while(head != NULL) {
        if (length == position) {
            return head;
        }
		length++;
		head = head->next;
	}
    return NULL;
}

struct dataBoard *setupDataBoard() {
	int side_template[8] = {1, 2, 3, 4, 5, 3, 2, 1};
	struct dataBoard *newBoard = malloc(sizeof(struct dataBoard));
	//fills in the board with the standard configuration
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			newBoard->board[row][col] = row <= 1 ? 0 : (row >= 6 ? 6 : 0);
			if (row == 0 || row == 7) { 
			    newBoard->board[row][col] += side_template[col];
			}
			else if(row != 6 && row != 1) {
			    newBoard->board[row][col] = 31;
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

char TeamOnSquare(const struct dataBoard *input_board, int row, int col) {
	if (!validRange(row) || !validRange(col)) {
		return ' ';
	}
	return sideOfDataPiece(getDataPiece(input_board, row, col));
}

bool kingExists(const struct dataBoard *input_board, char side) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieceIdOfDataPiece(getDataPiece(input_board, i, j)) == 'K' && sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
				return true;
			}
		}
	}
	return false;
}