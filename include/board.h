#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

#include "move.h"


struct dataPiece {
	uint8_t id;// i know this does not save any space but it might in the future ???

	/*
	formula:
	values:
	WP   = 0
	WR   = 1
	WN   = 2
	WB   = 3
	WQ   = 4
	WK   = 5
	
	BP   = 6
	BR   = 7
	BN   = 8
	BB   = 9
	BQ   = 10
	BK   = 11
    
	these are unimplemented

	EWP  = 12
	EBP  = 13
	UWK  = 14
	UBK  = 15
	UWR  = 16
	UBR  = 17
	
	else = 18-31
	
	*/ 
};

struct dataBoard {
	unsigned char board[8][8];
	//new board will be
	//
};

struct boardDiff {
	struct standardPos position1;
	struct dataPiece piece1;
	struct standardPos position2;
	struct dataPiece piece2;
};

struct piece {
	char pieceId;
	char side; //can be 'W' 'B' ' '
};

struct board {
    struct piece board[8][8]; 
};

struct boardCheck {
	uint64_t mask;
};




unsigned char getDataPiece(const struct dataBoard *board, signed char row, signed char col);
unsigned char *getDataPieceMutable(struct dataBoard *board, signed char row, signed char col);

bool isDataPieceSpecial(const unsigned char dp);
char sideOfDataPiece(const unsigned char dp);
float getBasicBoardScore(const struct dataBoard *input_board);
char pieceIdOfDataPiece(const unsigned char dp);
float valueOfDataPiece(const unsigned char dp);

struct boardDiff *boardDiffGenerator(const struct dataBoard *main_board, const struct dataBoard *different_board);
unsigned char makeDataPiece(char pieceId, char side, bool isSpecial);

bool doesTake(const struct dataBoard *input_board, struct standardPos *starting_position, struct standardPos *final_position);

struct dataPiece pieceToDataPiece(struct piece *p);
bool canCastle(const struct dataBoard *input_board, char side, bool is_king_side);


void printBoard(const struct board *input_board);

void printDataBoard(const struct dataBoard *input_board);
bool getBitOfBoardCheck(struct boardCheck *input_mask, unsigned char index);
uint64_t setBitOfBoardCheck(struct boardCheck *input_mask, unsigned char index);
unsigned char positionToIndex( char row,  char col);

void printDataBoardDebug(const struct dataBoard *input_board);
void printBoardCheck(struct boardCheck *input_mask);


struct board *setupBoard();

struct dataBoard *setupDataBoard();

bool validRange(int input);

char TeamOnSquare(const struct dataBoard *input_board, int row, int col);

char oppositeSide(char side);

void freeLinkedList(struct standardList *head);
int lengthOfLinkedList(struct standardList *head);
struct standardList *getElementOfLinkedList(struct standardList *head, int position);

bool kingExists(const struct dataBoard *input_board, char side);


#endif
