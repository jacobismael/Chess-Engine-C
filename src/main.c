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

struct dataBoard* mainBoard;
static sig_atomic_t continueRunning = 1;
bool* status;
char* input;

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

void getWhiteMove(struct dataBoard* mainBoard, bool* status) {
    // printf("\nPlayer 1 Move: ");
    // scanf("%s", input);
    
    // buildFromHalfMove(mainBoard, stringToFullDataTurn(mainBoard, input, 'W', status), 'W', status);
    buildFromHalfMove(mainBoard, bot1Choice(mainBoard, 'W', status), 'W', status);
    printDataBoard(mainBoard);
    if (isMate(mainBoard, 'B')) {
        printf("Player 1 wins\nMate!\n");
        exit_function();
    }
    if (isDraw(mainBoard, 'B')) {
        printf("Draw!\n");
        exit_function();
    }
    // printf("status: %d\n", *status);
}

void getBlackMove(struct dataBoard* mainBoard, bool* status) {
    // printf("\nPlayer 2 Move: ");
    //scanf("%s", input);
    
    buildFromHalfMove(mainBoard, bot2Choice(mainBoard, 'B', status), 'B', status);
    printDataBoard(mainBoard);
    if (isMate(mainBoard, 'W')) {
        printf("Player 2 wins\nMate!\n");
        exit_function();
    }
    if (isDraw(mainBoard, 'W')) {
        printf("Draw!\n");
        exit_function();
    }
    // printf("status: %d\n", *status);
}

int main(int argc, char** argv) {
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

    printDataBoard(mainBoard);

    status = malloc(sizeof(int));
    *status = 0;
    while(continueRunning) {
        move_number++;
        *status = 0;
        while(*status == 0) { 
            getWhiteMove(mainBoard, status); 

        }
        

        *status = 0;
        while(*status == 0) { 
            getBlackMove(mainBoard, status); 
        }
        printf("move_number: %d\n", move_number);
    }

    free(status);
    free(mainBoard);

    return 0;
}
