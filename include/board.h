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

struct dataBoard {
	unsigned char board[8][8];
	//new board will be
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

struct boardCheck {
	uint64_t mask;
};




void setDataPiece(struct dataBoard *board, signed char row, signed char col, unsigned char value);
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

bool canCastle(const struct dataBoard *input_board, char side, bool is_king_side);

void printDataBoard(const struct dataBoard *input_board, const bool borderless);
bool getBitOfBoardCheck(struct boardCheck *input_mask, unsigned char index);
uint64_t setBitOfBoardCheck(struct boardCheck *input_mask, unsigned char index);
unsigned char positionToIndex( char row,  char col);

void printDataBoardDebug(const struct dataBoard *input_board);
void printBoardCheck(struct boardCheck *input_mask);

struct dataBoard *setupDataBoard();

bool validRange(int input);

char TeamOnSquare(const struct dataBoard *input_board, int row, int col);

char oppositeSide(char side);

bool kingExists(const struct dataBoard *input_board, char side);


#endif
