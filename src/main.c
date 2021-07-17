#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include "pgn-functions.h"
#include "move.h"
#include "game.h"
#include "board.h"
#include "bot1.h"
#include "bot2.h"
#include "bot3.h"

struct dataBoard *mainBoard;
static sig_atomic_t continueRunning = 1;
bool *status;
char *input;

static void exit_function() {
    continueRunning = 0;
    free(mainBoard);
    free(status);
    free(input);
    exit(0);

}

static void signal_handler(int _) {
    (void)_;
    printf("\n");
    exit_function();
}

void checkForGameEnd(const struct dataBoard *input_board, char side) {
    if (isMate(input_board, 'B')) {
        printf("Player 1 wins\nMate!\n");
        exit_function();
    }
    if (isDraw(input_board, 'B')) {
        printf("Draw!\n");
        exit_function();
    }
}

void getWhiteMove(struct dataBoard *mainBoard, bool *status) {
    // scanf("%s", input);
    // choice = buildFromHalfMove(mainBoard, stringToFullDataTurn(mainBoard, input, 'W', status), 'W', status);
    struct fullDataTurn *choice = bot3Choice(mainBoard, 'W', status);
    if (choice == NULL) {
        *status = false;
        return;
    }
    printf("W is playing:\n");
    // printf("choice: \n%d %d : %d %d\n", choice->starting_position.row, choice->starting_position.col, choice->final_position.row, choice->final_position.col);
    buildFromHalfMove(mainBoard, choice, 'W', status);
    printMove(choice);
    printDataBoard(mainBoard, true);
    free(choice);
    
}

void getBlackMove(struct dataBoard *mainBoard, bool *status) {
    // printf("\nPlayer 2 Move: ");
    //scanf("%s", input);
    struct fullDataTurn *choice = bot1Choice(mainBoard, 'B', status);
    if (choice == NULL) {
        *status = false;
        return;
    }
    printf("B is playing:\n");
    buildFromHalfMove(mainBoard, choice, 'B', status);
    printDataBoard(mainBoard, true);
    free(choice);
}

int main(int argc, char **argv) {
    signal(SIGINT, signal_handler);
    srand(time(NULL));
    input = malloc(6);
    int move_number = 0;

    if(argc == 1) {
        // visual mode
        printf("Visual Mode\n");
        return 0;
    }

    else if(argc != 4) {
        printf("Need Event Name\n");
        return 0;
    }

    mainBoard = setupDataBoard();

    printDataBoard(mainBoard, true);

    status = malloc(sizeof(int));
    *status = 0;
    while(move_number != -10) {
        move_number++;
        *status = 0;
        while(*status == 0) { 
            getWhiteMove(mainBoard, status); 

        }
        checkForGameEnd(mainBoard, 'W');
        

        *status = 0;
        while(*status == 0) { 
            getBlackMove(mainBoard, status); 
        }
        checkForGameEnd(mainBoard, 'B');
        printf("move_number: %d\n", move_number);
    }

    free(status);
    free(mainBoard);

    return 0;
}
