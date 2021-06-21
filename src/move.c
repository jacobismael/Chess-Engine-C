#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "move.h"

struct Move* AddMove(struct Move* head, char* w_notation, char* b_notation) {
    struct Move* head_cpy = head;
    int position = 2;
    while(head->next != NULL) {
        head = head->next;
        position++;
    }
    struct Move* new_move = malloc(sizeof(struct Move));
    new_move->next = NULL;
    strncpy(new_move->white_notation , w_notation, strlen(w_notation));
    strncpy(new_move->black_notation , b_notation, strlen(b_notation));
    new_move->move_number = position;

    head->next = new_move;

    return head_cpy;
}


char* getMoveList(struct Move* head) {

    char* output = malloc(1000);
    char* buff = malloc(200);

    while(head != NULL) {
        sprintf(buff, "%d. %s %s ", head->move_number, head->white_notation, head->black_notation);
        strncat(output, buff, strlen(buff));
        head = head->next;
    }

    return output;
}


void PrintMoveList(struct Move* head) {
    while(head != NULL) {
        printf("%d. %s %s ", head->move_number, head->white_notation, head->black_notation);
        head = head->next;
    }
}


