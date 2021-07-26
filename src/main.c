#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <assert.h>

#include "pgn-functions.h"
#include "move.h"
#include "game.h"
#include "board.h"
#include "bot1.h"
#include "bot2.h"
#include "bot3.h"
#include "bot4.h"


typedef struct fullDataTurn* (trnptr)(const struct dataBoard *, char, bool *);

struct sides {
    trnptr *white;
    trnptr *black;
};

struct sides *chosen_sides;
struct dataBoard *main_board;
static sig_atomic_t continueRunning = 1;
bool *status;
char *input;

static void exit_function() {
    continueRunning = 0;
    free(main_board);
    free(status);
    free(chosen_sides);
    free(input);
    exit(0);
}

static void signal_handler(int _) {
    (void)_;
    printf("\n");
    exit_function();
}

void checkForGameEnd(const struct dataBoard *input_board, char side) {
    if (isMate(input_board, oppositeSide(side)))
    {
        printf("%c Checkmate!\n", side);
        exit_function();
    }
    if (isDraw(input_board, oppositeSide(side)))
    {
        printf("Draw!\n");
        exit_function();
    }
}

struct fullDataTurn *playerMove(const struct dataBoard *main_board, char side, bool *status) {
    printf("\n%s Move: ", side == 'W' ? "White" : "Black");
    scanf("%s", input);
    struct fullDataTurn *choice = stringToFullDataTurn(main_board, input, side, status);
    return choice;
}

void makeMove(struct dataBoard *main_board, char side, trnptr *func_ptr, bool *status) {
    struct fullDataTurn *choice = NULL;
    choice = func_ptr(main_board, side, status);
    if (choice == NULL) {
        *status = false;
        return;
    }
    main_board = buildFromHalfMove(main_board, choice, side, status);
    free(choice);
    //assert(status);
}


struct sides *getFlags(int argc, char **argv) {
    struct sides *result = malloc(sizeof(struct sides));
    result->white = NULL;
    result->black = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            assert(argv[i][1] == 'b' || argv[i][1] == 'w');
            assert(strlen(argv[i]) == 2);

            if (argv[i][1] == 'b') {
                if (!strcmp(argv[i + 1], "player")) {
                    result->black = &playerMove;
                }
                else if (!strcmp(argv[i + 1], "bot1")) { 
                    result->black = &bot1Choice;
                    printf("yeet");
                }
                else if (!strcmp(argv[i + 1], "bot2")) {
                    result->black = &bot2Choice;
                }
                else if (!strcmp(argv[i + 1], "bot3")) {
                    result->black = &bot3Choice;
                }
                else if (!strcmp(argv[i + 1], "bot4")) {
                    result->black = &bot4Choice;
                }
                else {
                    printf("unrecognised player\n");
                }
            }
            else if (argv[i][1] == 'w') {
                if (!strcmp(argv[i + 1], "player")) {
                    result->white = &playerMove;
                }
                else if (!strcmp(argv[i + 1], "bot1")) { 
                    printf("yeet");
                    result->white = &bot1Choice;
                }
                else if (!strcmp(argv[i + 1], "bot2")) {
                    result->white = &bot2Choice;
                }
                else if (!strcmp(argv[i + 1], "bot3")) {
                    result->white = &bot3Choice;
                }
                else if (!strcmp(argv[i + 1], "bot4")) {
                    result->white = &bot4Choice;
                }
                else {
                    printf("unrecognised player\n");
                }
            }
        }
    }
    if (result->white == NULL) {
        printf("white defaulting\n");
        result->white = &playerMove;
        
    }
    if (result->black == NULL) {
        printf("black defaulting\n");
        result->black = &playerMove;
    } 
    return result;
}

int main(int argc, char **argv) {
    signal(SIGINT, signal_handler);
    srand(time(NULL));
    input = malloc(6);
    int move_number = 0;
    chosen_sides = getFlags(argc, argv);

    main_board = setupDataBoard();

    printDataBoard(main_board, true);


    status = malloc(sizeof(bool));
    *status = false;
    while (move_number != -5) {
        move_number++;
        *status = false;

        while (!*status) {
            makeMove(main_board, 'W', chosen_sides->white, status);
        }
        printDataBoard(main_board, true);
        checkForGameEnd(main_board, 'W');

        *status = false;
        while (!*status) {
            makeMove(main_board, 'B', chosen_sides->black, status);
        }
        printDataBoard(main_board, true);
        checkForGameEnd(main_board, 'B');
        
        
        printf("move_number: %d\n", move_number);
    }
    free(chosen_sides);
    free(status);
    free(main_board);

    return 0;
}