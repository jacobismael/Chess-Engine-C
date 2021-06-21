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

int letterToCol(char letter) {
	if ((int)letter <= 96 || (int)letter >= 105) {
		printf("letters should be between a-h. You inputed %c", letter);
		exit(0);
	}
	return (int)letter - 97;
}

int ColToLetter(int col) {
	if ((int)col < 0 || (int)col > 7) {
		printf("letters should be between 0-7. You inputed %c", col);
		exit(0);
	}
	return (int)col + 97;
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

struct board* buildFromStart(struct board* inputBoard, struct Move* head) {
	while (head != NULL) {
		// head->white_notation;
		
		if (head->white_notation[0] == "K") {
 
		}
		
		
		// head->black_notation;


		
		
		head = head->next;
	}
	return inputBoard;
}

char TeamOnSquare(struct board* inputBoard, int row, int col) {
	return inputBoard->board[row][col].side;
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

char oppositeSide(char side) {  // maybe ths should overload some operator idk
	if (strchr("WB", side)) {
		return side == 'W' ? 'B' : 'W';
	}
	return side; //for no side
}

struct pos* listOfLegalMoves(struct board* inputBoard, struct pos* position) {
	struct pos* validPositions = NULL;
	char pieceType = inputBoard->board[position->row][position->col].pieceId;
	char side = inputBoard->board[position->row][position->col].side;
	// printPosList(position);

	if (pieceType == ' ') {
		return NULL;
	}

	if (pieceType == 'P') { // other pieces wont have usch hardcoded values
		short pawnDir = side == 'W' ? 1 : -1;
		validPositions = malloc(sizeof(struct pos));
		validPositions->next = NULL; // the first one is empty to use as a head and is not returned
		// printf("opposite side %c\n", oppositeSide(side));
		// printf("row %d\n", position->row);
		if (position->row == 1) { // for moving forward two steps if on row 2
			if (TeamOnSquare(inputBoard, position->row + 2*pawnDir, position->col) == ' ') { 
				appendPos(validPositions, position->row + 2*pawnDir, position->col);
			}
			else {
				printf("team on square = %c\n", TeamOnSquare(inputBoard, position->row + 2*pawnDir, position->col));
			}
		}
		if (TeamOnSquare(inputBoard, position->row + 1*pawnDir, position->col) == ' ') {
			appendPos(validPositions, position->row + 1*pawnDir, position->col);
		}
		if (TeamOnSquare(inputBoard, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) {
			appendPos(validPositions, position->row + 1*pawnDir, position->col - 1);
			printf("left forward\n");
		}
		if (TeamOnSquare(inputBoard, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) {
			appendPos(validPositions, position->row + 1*pawnDir, position->col + 1);
			printf("right forward\n");
		}
	}
	return validPositions->next; // we return next because the first value is just used  to set it up and has no real value
}

struct board* buildFromMove(struct board* inputBoard, struct Move* move) {
	//white's move
	char white_move[6];
	strncpy(white_move, move->white_notation, 6);

	char new_row = white_move[strlen(white_move) - 1];
	char new_col = white_move[strlen(white_move) - 2];
	
	if (strchr(white_move, 'x')) { // adds replaced peeice to list of pieces that are off the board
		// printf("nrow: %c\nncol: %c\n", new_row, new_col);
		inputBoard->off_the_board[0] = inputBoard->board[new_row - '0'][letterToCol(new_col)];	
	}
	char* list_of_valid_chars = "KQBNR";
	char new_piece_id = strchr(list_of_valid_chars, white_move[0]) ? white_move[0]: 'P';
	struct pos temp_position = {.row = 0, .col = 0};
	struct pos* lolm = malloc(sizeof(struct pos));
	struct pos expected_result = {.row = new_row - '0' - 1, .col = letterToCol(new_col), .next=NULL};


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (inputBoard->board[i][j].pieceId == new_piece_id && inputBoard->board[i][j].side == 'W') {
				struct pos temp_position = {.row = i, .col = j, .next = NULL};

				lolm = listOfLegalMoves(inputBoard, &temp_position);
				// printf("%c %d\n", new_col, letterToCol(new_col));
				
				if (posLlContains(lolm, &expected_result)) {	
					inputBoard->board[i][j].pieceId = ' ';
					inputBoard->board[i][j].side = ' ';
					
					break;
				}
			}
		}
	}
	
	struct piece temp_piece = {.pieceId=new_piece_id, .side='W'};
	inputBoard->board[new_row - '0' - 1][letterToCol(new_col)] = temp_piece; // the -1 is required because arrs start at 0
	
	return inputBoard;
}


bool posLlContains(struct pos* head, struct pos* to_compare) {
	while(head != NULL) {
		// printf("%p\n", head->next);
		if (head->row == to_compare->row && head->col == to_compare->col) {
			return true;
		}	
		else {
			printf("row %d col %d\n", head->row, head->col);
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
