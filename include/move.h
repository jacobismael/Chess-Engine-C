#include <stdbool.h>
#ifndef MOVE_H
#define MOVE_H

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

struct dataTurn {
    struct standard_pos restrictors;
    struct standard_pos final_position;
    char piece;
    bool takes;
    bool castles;
    bool is_king_side;
    bool is_check;
};

struct standard_pos posToStandard_pos(struct pos* input_pos);

struct pos standard_posToPos(struct standard_pos* input_pos);

int letterToCol(char letter);
int ColToLetter(int col);
struct pos* getRestrictors(char* move);
struct dataTurn* toDataTurn(char* main);
struct Move* AddMove(struct Move* head, char* w_notation, char* b_notation);
char* getMoveList(struct Move* head);
void PrintMoveList(struct Move* head);

void printPosList(struct pos* head);

struct pos* appendPos(struct pos* head, int row, int col);
void freePosList(struct pos* head);
bool posLlContains(struct pos* head, struct pos* to_compare);

#endif