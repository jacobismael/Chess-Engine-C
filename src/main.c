#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgn-functions.h"
#include "move.h"
#include "board.h"


int main(int argc, char** argv) {

    if(argc == 1) {
        // visual mode
        printf("Visual Mode\n");
        return 0;
    }

    if(argc != 4) {
        printf("Need Event Name\n");
        return 0;
    }


    struct PGN* game = malloc(sizeof(struct PGN));

    strncpy(game->event,  argv[1], strlen(argv[1]));
    strncpy(game->site, argv[2], strlen(argv[2]));
    strncpy(game->date, "2021.06.18", 10);
    game->round = 1;
    strncpy(game->white , "Jacob", 5);
    strncpy(game->black , "Atli", 4);


    struct Move* head_move = malloc(sizeof(struct Move));
 
    strncpy(head_move->white_notation , "e4", 2);
    strncpy(head_move->black_notation , "e5", 2);
    head_move->move_number = 1;
    head_move->next = NULL;

    head_move = AddMove(head_move, "Nf3", "Nc6");
    head_move = AddMove(head_move, "Bb5", "a6");
    head_move = AddMove(head_move, "Nf3", "Nc6");
    game->move_head = head_move;

    game->result = MakeResult(1, 0);

    //PrintGame(game);
    PrintGameStrict(game);
    printf("===================================\n\n");
    SavePGNtoFile(game, argv[3]);

    struct board* mainBoard = setupBoard();
    printBoard(mainBoard);
    buildFromMove(mainBoard, head_move);
    printBoard(mainBoard);


    return 0;
}
