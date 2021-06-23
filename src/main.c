#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgn-functions.h"
#include "move.h"
#include "board.h"


void getWhiteMove(struct board* mainBoard, int* status) {
    char* input = malloc(sizeof(char) * 6);
    printf("\nPlayer 1 Move: ");
    scanf("%s", input);
    buildFromHalfMove(mainBoard, input, 'W', status);
    free(input);
}

void getBlackMove(struct board* mainBoard, int* status) {
    char* input = malloc(sizeof(char) * 6);
    printf("\nPlayer 2 Move: ");
    scanf("%s", input);
    buildFromHalfMove(mainBoard, input, 'B', status);
    free(input);
}


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


    struct Turn* player1move = malloc(sizeof(struct Turn));
    struct Turn* player2move = malloc(sizeof(struct Turn));

    struct board* mainBoard = setupBoard();
    printBoard(mainBoard);
    // buildFromStart(mainBoard, head_move);
    // printBoard(mainBoard);

    int* status = malloc(sizeof(int));

    *status = 0;
    while(1) {
        *status = 0;
        while(*status == 0) { 
            getWhiteMove(mainBoard, status); 
        }
        printBoard(mainBoard);

        *status = 0;
        while(*status == 0) { 
            getBlackMove(mainBoard, status); 
        }

        printBoard(mainBoard);
    }


    // struct PGN* game = malloc(sizeof(struct PGN));

    // strncpy(game->event,  argv[1], strlen(argv[1]));
    // strncpy(game->site, argv[2], strlen(argv[2]));
    // strcpy(game->date, "2021.06.18");
    // game->round = 1;
    // strcpy(game->white , "Jacob");
    // strcpy(game->black , "Atli");g

    // struct Move* head_move = malloc(sizeof(struct Move));
    // head_move->move_number = 1;
    // head_move->next = NULL;
 
    // strcpy(head_move->white_notation , "e4");
    // strcpy(head_move->black_notation , "e5");
    // head_move = AddMove(head_move, "Nf3", "Nc6");
    // head_move = AddMove(head_move, "Bb5", "a6");
    // head_move = AddMove(head_move, "Ba4", "Nf6");
    // head_move = AddMove(head_move, "O-O", "Be7");
    // head_move = AddMove(head_move, "Re1", "b5");
    // head_move = AddMove(head_move, "Bb3", "d6");
    // head_move = AddMove(head_move, "c3", "O-O");    
    // head_move = AddMove(head_move, "h3", "Nb8");
    // head_move = AddMove(head_move, "d4", "Nbd7");


    // game->move_head = head_move;

    // game->result = MakeResult(1, 0);

    //PrintGame(game);
    // PrintGameStrict(game);
    // printf("===================================\n\n");
    // SavePGNtoFile(game, argv[3]);

    // struct board* mainBoard = setupBoard();
    // printBoard(mainBoard);
    // buildFromStart(mainBoard, head_move);
    // printBoard(mainBoard);



    return 0;
}
