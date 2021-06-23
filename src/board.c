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

struct pos* findPos(struct board* input_board, struct piece* piece_to_find, char restrictor_row, char restrictor_col) {
	int row_min = restrictor_row == 'z' ? 0 : restrictor_row - '0'; //z is used as a special char to indicate no restriction on search 
	int row_max = restrictor_row == 'z' ? 8 : restrictor_row - '0'; //z is used as a special char to indicate no restriction on search 


	int col_min = restrictor_col == 'z' ? 0 : letterToCol(restrictor_row); //z is used as a special char to indicate no restriction on search 
	int col_max = restrictor_col == 'z' ? 8: letterToCol(restrictor_row); //z is used as a special char to indicate no restriction on search 

	for (int row = row_min; row  < row_max; row++) {
		for (int col = col_min; col  < col_max; col++) {
			if (input_board->board[row][col].pieceId == piece_to_find->pieceId && input_board->board[row][col].side == piece_to_find->side) {
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

char TeamOnSquare(struct board* input_board, int row, int col) {
	return input_board->board[row][col].side;
}

char oppositeSide(char side) {  // maybe this should overload some operator idk
	if (strchr("WB", side)) {
		return side == 'W' ? 'B' : 'W';
	}
	return side; //for no side
}

struct pos* pawnMovement(struct board* input_board, struct pos* validPositions, struct pos* position, char side) {
	short pawnDir = side == 'W' ? 1 : -1;
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	if ((position->row == 1 && side == 'W') || (position->row == 6 && side == 'B')) { // for moving forward two steps if on row 2
		if (TeamOnSquare(input_board, position->row + 2*pawnDir, position->col) == ' ') { 
			appendPos(validPositions, position->row + 2*pawnDir, position->col);
		}
	}
	if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col) == ' ') {
		appendPos(validPositions, position->row + 1*pawnDir, position->col);
	}
	if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) {
		appendPos(validPositions, position->row + 1*pawnDir, position->col - 1);
		
	}
	if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) {
		appendPos(validPositions, position->row + 1*pawnDir, position->col + 1);
		
	}
	return validPositions;
}

struct pos* knightMovement(struct board* input_board, struct pos* validPositions, struct pos* position, char side) {
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

			if (strchr(combined, input_board->board[new_row + (side == 'W' ? 0 : /*-1*/ 0)][new_col].pieceId)) {

				appendPos(validPositions, new_row, new_col);
			}
		}
		knight_pos_offset_head = knight_pos_offset_head->next;
	}
	return validPositions;
}

struct pos* bishopMovement(struct board* input_board, struct pos* validPositions, struct pos* position, char side) {
	
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			char combined[] = { ' ', oppositeSide(side)};
			if (strchr(combined, input_board->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId)) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (input_board->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	
	return validPositions;
}

struct pos* rookMovement(struct board* input_board, struct pos* validPositions, struct pos* position, char side) {
	
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			char combined[] = { ' ', oppositeSide(side)};
			if (strchr(combined, input_board->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId)) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (input_board->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}

struct pos* queenMovement(struct board* input_board, struct pos* validPositions, struct pos* position, char side) {
	
	validPositions->next = NULL; // the first one is empty to use as a head and is not returned
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < 8; i ++) {
		for (int j = 1; j < 8; j++) {
			char combined[] = { ' ', oppositeSide(side)};
			if (strchr(combined, input_board->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId)) {
				appendPos(validPositions, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]);
				if (input_board->board[position->row + j*multipliers[i][0]][position->col + j*multipliers[i][1]].pieceId == oppositeSide(side)) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}



struct pos* listOfLegalMoves(struct board* input_board, struct pos* position) {
	struct pos* validPositions = malloc(sizeof(struct pos));
	char pieceType = input_board->board[position->row][position->col].pieceId;
	char side = input_board->board[position->row][position->col].side;

	switch(pieceType) {
		case ' ':
			return NULL;
		case 'P': // other pieces wont have usch hardcoded values
			validPositions = pawnMovement(input_board, validPositions, position, side);
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
			validPositions = rookMovement(input_board, validPositions, position, side);
			break;
	}
	return validPositions->next; // we return next because the first value is just used  to set it up and has no real value
}


struct board* buildFromStart(struct board* input_board, struct Move* head) {
	while (head != NULL) {
		input_board = buildFromMove(input_board, head);
		printBoard(input_board);
		
		
		head = head->next;
	}
	return input_board;
}


struct board* buildFromMove(struct board* input_board, struct Move* move) {
	//white's move
	char white_move[6];
	strncpy(white_move, move->white_notation, 6);
	

	input_board = buildFromHalfMove(input_board, white_move, 'W', NULL);
	
	char black_move[6];
	strncpy(black_move, move->black_notation, 6);
	input_board = buildFromHalfMove(input_board, black_move, 'B', NULL);


	return input_board;
}

struct board* buildFromHalfMove(struct board* input_board, char* move, char side, int* status) {
	*status = 0;
	struct dataTurn* cmove = toDataTurn(move);
	//castle handling
	if (cmove->castles ==  1) {
		int end_row = side == 'W' ? 0 : 7;
		struct piece blank_piece = {.pieceId = ' ', .side = ' '};
		struct piece king_piece = {.pieceId = 'K', .side = side};
		struct piece rook_piece = {.pieceId = 'R', .side = side};
		if (cmove->is_king_side) {
			if (input_board->board[end_row][5].pieceId == ' ' && input_board->board[end_row][6].pieceId == ' ') { // checks that the squares between the king are empty
				// printf("here\n");
				if(input_board->board[end_row][4].pieceId == 'K' && input_board->board[end_row][7].pieceId == 'R') {
					*status = 1;
					
					input_board->board[end_row][4] = blank_piece;
					input_board->board[end_row][5] = rook_piece;
					input_board->board[end_row][6] = king_piece;
					input_board->board[end_row][7] = blank_piece;
				}
			}
		}
		else {
			if (input_board->board[end_row][1].pieceId == ' ' && input_board->board[end_row][2].pieceId == ' '  && input_board->board[end_row][3].pieceId == ' ') { // checks that the squares between the king are empty
				// printf("here\n");
				if(input_board->board[end_row][4].pieceId == 'K' && input_board->board[end_row][0].pieceId == 'R') {
					*status = 1;
					
					input_board->board[end_row][0] = blank_piece;
					input_board->board[end_row][1] = blank_piece;
					input_board->board[end_row][2] = king_piece;
					input_board->board[end_row][3] = rook_piece;
					input_board->board[end_row][4] = blank_piece;
				}
			}
		}

		return input_board;
	}
	
	if (cmove->takes) { // adds replaced peice to list of pieces that are off the board
		input_board->off_the_board[0] = input_board->board[cmove->final_position.row][cmove->final_position.col];	
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

				if (input_board->board[i][j].pieceId == cmove->piece && input_board->board[i][j].side == side) {
					temp_position.row = i;
					temp_position.col = j, 
					temp_position.next = NULL;

					lolm = listOfLegalMoves(input_board, &temp_position);
					if (posLlContains(lolm, &expected_result)) {
						input_board->board[i][j].pieceId = ' ';
						input_board->board[i][j].side = ' ';
						*status = 1;
						break;
					}
				}
			}
		}
	}
	if(*status == 1) {
		struct piece temp_piece = {.pieceId=cmove->piece, .side=side};
		input_board->board[cmove->final_position.row - 1][cmove->final_position.col] = temp_piece; // the -1 is required because arrs start at 0
		printf("yeet\n");
	}
	return input_board;
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

void printBoard(struct board* input_board) {
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
		else
		{
			printf("\n");
		}
		
	}

	printf("   a  b  c  d  e  f  g  h\n");
}

void printPosList(struct pos* head) {
	while (head != NULL) {
		printf("row %d, col %d\n", head->row, head->col);
		head = head->next;
	}

}
