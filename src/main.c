#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include "pgn-functions.h"
#include "move.h"
#include "game.h"
#include "board.h"

struct dataBoard* mainBoard;
static sig_atomic_t continueRunning = 1;
int* status;
char* input;

static void signal_handler(int _) {
    (void)_;
    continueRunning = 0;
    free(mainBoard);
    free(status);
    free(input);
    exit(0);
}

void getWhiteMove(struct dataBoard* mainBoard, int* status) {
    printf("\nPlayer 1 Move: ");
    
    scanf("%s", input);
    
    buildFromHalfMove(mainBoard, input, 'W', status);
}

void getBlackMove(struct dataBoard* mainBoard, int* status) {
    printf("\nPlayer 2 Move: ");

    scanf("%s", input);
    
    buildFromHalfMove(mainBoard, input, 'B', status);
}

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);
    input = malloc(sizeof(char) * 6);

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
        *status = 0;
        while(*status == 0) { 
            getWhiteMove(mainBoard, status); 

        }
        printDataBoard(mainBoard);
        
        *status = 0;
        while(*status == 0) { 
            getBlackMove(mainBoard, status); 
        }

        printDataBoard(mainBoard);
    }

    free(status);
    free(mainBoard);

    return 0;
}
