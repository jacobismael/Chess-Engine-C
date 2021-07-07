#ifndef MOVE_H
#define MOVE_H
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct pos { 
	int row:4;
	int col:4;
	struct pos*  next; //optional
};


struct standard_pos { 
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

struct basicDataTurnNode {
	struct standard_pos starting_pos;
	struct standard_pos ending_pos;
	struct basicDataTurnNode* next;
};

struct dataTurn {
    struct standard_pos restrictors;
    struct standard_pos final_position;
    char piece;
    bool takes;
    bool castles;
    bool is_king_side;
    bool is_check;
    char piece_promotes_to;
};

struct fullDataTurn {
    struct standard_pos final_position;
    struct standard_pos starting_position;
    char piece;
    bool is_en_passant;
    bool is_special;
    bool takes;
    bool castles;
    bool is_king_side;
    bool is_check;

    char piece_promotes_to;
};

struct standard_pos posToStandard_pos(const struct pos* input_pos);

struct pos standard_posToPos(const struct standard_pos* input_pos);

int letterToCol(char letter);
int ColToLetter(int col);
struct pos* getRestrictors(char* move);
struct dataTurn* toDataTurn(const char* main);
struct Move* AddMove(struct Move* head, char* w_notation, char* b_notation);

struct basicDataTurnNode* appendBasicDataTurn(struct basicDataTurnNode* head, struct standard_pos* starting_pos, struct standard_pos* ending_pos);
int lengthOfBasicDataTurn(struct basicDataTurnNode* head);
struct basicDataTurnNode* getElementOfBasicDataTurn(struct basicDataTurnNode* head, int position);
void freeBasicDataTurn(struct basicDataTurnNode* head);

char* getMoveList(struct Move* head);
void PrintMoveList(struct Move* head);

void printPosList(struct pos* head);

void appendPos(struct pos* head, int row, int col);
void freePosList(struct pos* head);
bool posLlContains(struct pos* head, struct pos* to_compare);

#endif
