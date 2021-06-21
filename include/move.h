#ifndef MOVE_H
#define MOVE_H

struct Move {

    int move_number;
    char white_notation[6];
    char black_notation[6];

    struct Move* next;
};

struct Move* AddMove(struct Move* head, char* w_notation, char* b_notation);
char* getMoveList(struct Move* head);
void PrintMoveList(struct Move* head);

#endif