#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pgn-functions.h"
#include "move.h"
#include "game.h"

int continueRunning = 1;


void getWhiteMove(struct dataBoard* mainBoard, int* status) {
    char* input = malloc(sizeof(char) * 6);
    printf("\nPlayer 1 Move: ");
    scanf("%s", input);
    if (input[0] == 'q' && strlen(input) == 1) {
        continueRunning = 0;
        *status = 1;
        free(input);
        return;
    }
    buildFromHalfMove(mainBoard, input, 'W', status);
    free(input);
}

void getBlackMove(struct dataBoard* mainBoard, int* status) {
    char* input = malloc(sizeof(char) * 6);
    printf("\nPlayer 2 Move: ");
    scanf("%s", input);
    if (input[0] == 'q' && strlen(input) == 1) {
        continueRunning = 0;
        *status = 1;
        free(input);
        return;
    }
    buildFromHalfMove(mainBoard, input, 'B', status);
    free(input);
}

int main(int argc, char** argv) {

    if(argc == 1) {
        // visual mode
        printf("Visual Mode\n");
        return 0;
    }

    else if(argc != 4) {
        printf("Need Event Name\n");
        return 0;
    }

    struct dataBoard* mainBoard = setupDataBoard();

    printDataBoard(mainBoard);

    int* status = malloc(sizeof(int));

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
        printf("continue running %d\n", continueRunning);
    }

    free(status);
    free(mainBoard);

    return 0;
}
