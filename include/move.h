#include <stdbool.h>
#ifndef MOVE_H
#define MOVE_H

struct pos { 
	int row;
	int col;
	struct pos*  next; //optional
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
    struct pos restrictors;
    struct pos final_position;
    char piece;
    bool takes;
    bool castles;
    bool is_king_side;
    bool is_check;
};

int letterToCol(char letter);
int ColToLetter(int col);
struct pos* getRestrictors(char* move);
struct dataTurn* toDataTurn(char* main);
struct Move* AddMove(struct Move* head, char* w_notation, char* b_notation);
char* getMoveList(struct Move* head);
void PrintMoveList(struct Move* head);

#endif