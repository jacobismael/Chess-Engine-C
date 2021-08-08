#ifndef MOVE_H
#define MOVE_H
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Pos { 
	int row:4;
	int col:4;
	struct Pos*  next; //optional
};


struct standardPos { 
	int row:4;
	int col:4;
};

struct Turn {
    char side;
    char move[6];

};

struct Move {
    int move_number;
    char white_notation[6];
    char black_notation[6];

    struct Move* next;
};

struct basicDataTurn {
	struct standardPos starting_pos;
	struct standardPos ending_pos;
};

struct dataTurn {
    struct standardPos restrictors;
    struct standardPos final_position;
    char piece;
    bool takes;
    bool castles;
    bool is_king_side;
    bool is_check;
    char piece_promotes_to;
};

struct fullDataTurn {
    struct standardPos final_position;
    struct standardPos starting_position;
    char piece;
    bool is_en_passant;
    bool is_special;
    bool takes;
    bool castles;
    bool is_king_side;
    bool is_check;

    char piece_promotes_to;
};



struct standardPos posToStandard_pos(const struct Pos* input_pos);

struct Pos standard_posToPos(const struct standardPos* input_pos);

int letterToCol(char letter);
int ColToLetter(int col);
struct Pos* getRestrictors(char* move);
struct dataTurn* toDataTurn(const char* main);
struct Move* AddMove(struct Move* head, char* w_notation, char* b_notation);
void printMove(const struct fullDataTurn* node);

struct basicDataTurnNode* appendBasicDataTurn(struct basicDataTurnNode* head, struct standardPos* starting_pos, struct standardPos* ending_pos);
int lengthOfBasicDataTurn(struct basicDataTurnNode* head);
struct basicDataTurnNode* getElementOfBasicDataTurn(struct basicDataTurnNode* head, int position);
void freeBasicDataTurn(struct basicDataTurnNode* head);

char* getMoveList(struct Move* head);
void PrintMoveList(struct Move* head);

void printPosList(struct Pos* head);
void appendPos(struct Pos* head, int row, int col);
void freePosList(struct Pos* head);
bool posLlContains(struct Pos* head, struct Pos* to_compare);

#endif
