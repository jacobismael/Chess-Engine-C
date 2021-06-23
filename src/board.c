#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "board.h"

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
	for (int i = 0; i < 48; i++) {
		struct piece newPiece = {.pieceId=' ', .side=' '};
		newBoard->off_the_board[i] = newPiece;
	}
	return newBoard;
}

struct pos* findPos(struct board* inputBoard, struct piece* piece_to_find, char restrictor_row, char restrictor_col) {
	int row_min = restrictor_row == 'z' ? 0 : restrictor_row - '0'; //z is used as a special char to indicate no restriction on search 
	int row_max = restrictor_row == 'z' ? 8 : restrictor_row - '0'; //z is used as a special char to indicate no restriction on search 


	int col_min = restrictor_col == 'z' ? 0 : letterToCol(restrictor_row); //z is used as a special char to indicate no restriction on search 
	int col_max = restrictor_col == 'z' ? 8: letterToCol(restrictor_row); //z is used as a special char to indicate no restriction on search 

	for (int row = row_min; row  < row_max; row++) {
		for (int col = col_min; col  < col_max; col++) {
			if (inputBoard->board[row][col].pieceId == piece_to_find->pieceId && inputBoard->board[row][col].side == piece_to_find->side) {
				struct pos* newPos = malloc(sizeof(struct pos));
				newPos->row = row;
				newPos->col = col;
				return newPos;
			}
		}
	}

	return NULL;
}

struct pos* appendPos(struct pos* head, int row, int col){

	struct pos* headcpy = head;
	while(head->next != NULL) {
		head = head->next;
	}
	struct pos* newPos = malloc(sizeof(struct pos));
	newPos->next = NULL;
	newPos->row = row;
	newPos->col = col;
	head->next = newPos;
	return headcpy;
}

char TeamOnSquare(struct board* inputBoard, int row, int col) {
	return inputBoard->board[row][col].side;
}

char oppositeSide(char side) {  // maybe this should overload some operator idk
	if (strchr("WB", side)) {
		return side == 'W' ? 'B' : 'W';
	}
	return side; //for no side
}

struct pos* pawnMovement(struct board* inputBoard, struct pos* validPositions, struct pos* position, char side) {
	short pawnDir = side == 'W' ? 1 : -1;
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	if ((position->row == 1 && side == 'W') || (position->row == 6 && side == 'B')) { // for moving forward two steps if on row 2
		if (TeamOnSquare(inputBoard, position->row + 2*pawnDir, position->col) == ' ') { 
			appendPos(validPositions, position->row + 2*pawnDir, position->col);
		}
	}
	if (TeamOnSquare(inputBoard, position->row + 1*pawnDir, position->col) == ' ') {
		appendPos(validPositions, position->row + 1*pawnDir, position->col);
	}
	if (TeamOnSquare(inputBoard, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) {
		appendPos(validPositions, position->row + 1*pawnDir, position->col - 1);
		
	}
	if (TeamOnSquare(inputBoard, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) {
		appendPos(validPositions, position->row + 1*pawnDir, position->col + 1);
		
	}
	return validPositions;
}

struct pos* knightMovement(struct board* inputBoard, struct pos* validPositions, struct pos* position, char side) {
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
		if (new_row >= 0 && new_row <= 8 && new_col >= 0 && new_col <= 8) {
			char combined[] = { ' ', oppositeSide(side)};

			if (strchr(combined, inputBoard->board[new_row + (side == 'W' ? 0 : /*-1*/ 0)][new_col].pieceId)) {

				appendPos(validPositions, new_row, new_col);
			}
		}
		knight_pos_offset_head = knight_pos_offset_head->next;
	}
	return validPositions;
}

struct pos* bishopMovement(struct board* inputBoard, struct pos* validPositions, struct pos* position, char side) {
	
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			char combined[] = { ' ', oppositeSide(side)};
			if (strchr(combined, inputBoard->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId)) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (inputBoard->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	
	return validPositions;
}

struct pos* rookMovement(struct board* inputBoard, struct pos* validPositions, struct pos* position, char side) {
	
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			char combined[] = { ' ', oppositeSide(side)};
			if (strchr(combined, inputBoard->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId)) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (inputBoard->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}

struct pos* queenMovement(struct board* inputBoard, struct pos* validPositions, struct pos* position, char side) {
	
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < 8; i ++) {
		for (int j = 1; j < 8; j++) {
			char combined[] = { ' ', oppositeSide(side)};
			if (strchr(combined, inputBoard->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId)) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (inputBoard->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}



struct pos* listOfLegalMoves(struct board* inputBoard, struct pos* position) {
	struct pos* validPositions = malloc(sizeof(struct pos));
	char pieceType = inputBoard->board[position->row][position->col].pieceId;
	char side = inputBoard->board[position->row][position->col].side;

	switch(pieceType) {
		case ' ':
			return NULL;
		case 'P': // other pieces wont have usch hardcoded values
			validPositions = pawnMovement(inputBoard, validPositions, position, side);
			break;
		case 'N':
			validPositions = knightMovement(inputBoard, validPositions, position, side);
			break;
		case 'B':
			validPositions = bishopMovement(inputBoard, validPositions, position, side);
			break;
		case 'R':
			validPositions = rookMovement(inputBoard, validPositions, position, side);
			break;
	}
	return validPositions->next; // we return next because the first value is just used  to set it up and has no real value
}


struct board* buildFromStart(struct board* inputBoard, struct Move* head) {
	while (head != NULL) {
		inputBoard = buildFromMove(inputBoard, head);
		printBoard(inputBoard);
		
		
		head = head->next;
	}
	return inputBoard;
}


struct board* buildFromMove(struct board* inputBoard, struct Move* move) {
	//white's move
	char white_move[6];
	strncpy(white_move, move->white_notation, 6);
	

	inputBoard = buildFromHalfMove(inputBoard, white_move, 'W');
	
	char black_move[6];
	strncpy(black_move, move->black_notation, 6);
	inputBoard = buildFromHalfMove(inputBoard, black_move, 'B');


	return inputBoard;
}

struct board* buildFromHalfMove(struct board* inputBoard, char* move, char side) {
	struct dataTurn* cmove = toDataTurn(move);
	//castle handling
	if (cmove->castles ==  1) {
		int end_row = side == 'W' ? 0 : 7;
		struct piece blank_piece = {.pieceId = ' ', .side = ' '};
		struct piece king_piece = {.pieceId = 'K', .side = side};
		struct piece rook_piece = {.pieceId = 'R', .side = side};
		
		if (inputBoard->board[end_row][5].pieceId == ' ' && inputBoard->board[end_row][6].pieceId == ' ') { // checks that the squares between the king are empty
			// printf("here\n");
			if(inputBoard->board[end_row][4].pieceId == 'K' && inputBoard->board[end_row][7].pieceId == 'R') {
				inputBoard->board[end_row][4] = blank_piece;
				inputBoard->board[end_row][5] = rook_piece;
				inputBoard->board[end_row][6] = king_piece;
				inputBoard->board[end_row][7] = blank_piece;
			}
		}
		return inputBoard;
	}
	
	if (cmove->takes) { // adds replaced peice to list of pieces that are off the board
		inputBoard->off_the_board[0] = inputBoard->board[cmove->final_position.row][cmove->final_position.col];	
	}
	struct pos temp_position = {.row = 0, .col = 0};
	struct pos* lolm = malloc(sizeof(struct pos));
	struct pos expected_result = {.row = cmove->final_position.row -1 , .col = cmove->final_position.col, .next=NULL};
	struct pos* restrictors = &cmove->restrictors;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			if (restrictors == NULL || ((restrictors->row == i || restrictors->row == -1) && (restrictors->col == j || restrictors->col == -1))) { 
			// this is The line to focus on
			//also add a game.c with a game struct etc 

				if (inputBoard->board[i][j].pieceId == cmove->piece && inputBoard->board[i][j].side == side) {
					temp_position.row = i;
					temp_position.col = j, 
					temp_position.next = NULL;

					lolm = listOfLegalMoves(inputBoard, &temp_position);
					if (posLlContains(lolm, &expected_result)) {
						inputBoard->board[i][j].pieceId = ' ';
						inputBoard->board[i][j].side = ' ';
						
						break;
					}
				}
			}
		}
	}
	
	struct piece temp_piece = {.pieceId=cmove->piece, .side=side};
	inputBoard->board[cmove->final_position.row - 1][cmove->final_position.col] = temp_piece; // the -1 is required because arrs start at 0
	return inputBoard;
}

bool posLlContains(struct pos* head, struct pos* to_compare) {
	while(head != NULL) {
		if (head->row == to_compare->row && head->col == to_compare->col) {
			return true;
		}	
		else {
		}
		head = head->next;
	}
	return false;
}

void printBoard(struct board* inputBoard) {
	printf("\n\n\n");
	for (int i = 7; i >= 0; i--) {
		printf("%d  ", i+ 1);
		for (int j = 0; j < 8; j++) {
			printf("%c%c", inputBoard->board[i][j].side, inputBoard->board[i][j].pieceId);
			if (j != 7) { 
				printf("|");
			}
		}
		if (i != 0) {
			printf("\n   --+--+--+--+--+--+--+--\n");
		}
		else
		{
			printf("\n");
		}
		
	}
}

void printPosList(struct pos* head) {
	while (head != NULL) {
		printf("row %d, col %d\n", head->row, head->col);
		head = head->next;
	}

}
