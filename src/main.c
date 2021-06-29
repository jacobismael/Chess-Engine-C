#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgn-functions.h"
#include "move.h"
#include "board.h"


void getWhiteMove(struct dataBoard* mainBoard, int* status) {
    char* input = malloc(sizeof(char) * 6);
    printf("\nPlayer 1 Move: ");
    scanf("%s", input);
    buildFromHalfMove(mainBoard, input, 'W', status);
    free(input);
}

void getBlackMove(struct dataBoard* mainBoard, int* status) {
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

    else if(argc != 4) {
        printf("Need Event Name\n");
        return 0;
    }

    struct dataBoard* mainBoard = setupDataBoard();
    printf("%p \n", mainBoard);
    printf("test\n");
    printDataBoard(mainBoard);

    int* status = malloc(sizeof(int));

    *status = 0;
    while(1) {
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
