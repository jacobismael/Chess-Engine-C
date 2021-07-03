#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include "move.h"



struct dataPiece {
	unsigned char id:4;// i know this does not save any space but it might in the future ???
	/* 
	formula:
	values:
	WP = 0
	WR = 1
	WN = 2
	WB = 3
	WQ = 4
	WK = 5
	
	BP = 6
	BR = 7
	BN = 8
	BB = 9
	BQ = 10
	BK = 11

	   = else
	
	
	*/ 
};

struct dataBoard {
	struct dataPiece board[8][8];	
};

struct boardDiff {
	struct standard_pos position1;
	struct dataPiece piece1;
	struct standard_pos position2;
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


struct dataPiece getDataPiece(const struct dataBoard* board, signed char row, signed char col);
struct dataPiece* getDataPieceMutable(struct dataBoard* board, signed char row, signed char col);

char sideOfDataPiece(const struct dataPiece dp);
char pieceIdOfDataPiece(const struct dataPiece dp);

struct boardDiff* boardDiffGenerator(const struct dataBoard* main_board, const struct dataBoard* different_board);
struct dataPiece makeDataPiece(char pieceId, char side);

struct dataPiece pieceToDataPiece(struct piece* p);

void printBoard(const struct board* input_board);

void printDataBoard(const struct dataBoard* input_board);

bool getBitOfBoardCheck(struct boardCheck* input_mask, unsigned char index);
uint64_t setBitOfBoardCheck(struct boardCheck* input_mask, unsigned char index);
unsigned char positionToIndex( char row,  char col);

void printBoardCheck(struct boardCheck* input_mask);


struct board* setupBoard();

struct dataBoard* setupDataBoard();

bool validRange(int input);

char TeamOnSquare(const struct dataBoard* input_board, int row, int col);

char oppositeSide(char side);



#endif