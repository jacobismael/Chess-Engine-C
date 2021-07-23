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


typedef struct fullDataTurn* (*trnptr())(struct fullDataBoard *, char, bool *);

struct dataBoard *main_board;
static sig_atomic_t continueRunning = 1;
bool *status;
char *input;

static void exit_function()
{
    continueRunning = 0;
    free(main_board);
    free(status);
    free(input);
    exit(0);
}

static void signal_handler(int _)
{
    (void)_;
    printf("\n");
    exit_function();
}

void checkForGameEnd(const struct dataBoard *input_board, char side)
{
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

void getWhiteMove(struct dataBoard *main_board, bool *status)
{
    // scanf("%s", input);
    // choice = buildFromHalfMove(main_board, stringToFullDataTurn(main_board, input, 'W', status), 'W', status);
    struct fullDataTurn *choice = bot4Choice(main_board, 'W', status);
    if (choice == NULL)
    {
        *status = false;
        return;
    }
    printf("W is playing:\n");
    // printf("choice: \n%d %d : %d %d\n", choice->starting_position.row, choice->starting_position.col, choice->final_position.row, choice->final_position.col);
    printMove(choice);
    buildFromHalfMove(main_board, choice, 'W', status);
    printDataBoard(main_board, true);
    free(choice);
}

void getBlackMove(struct dataBoard *main_board, bool *status)
{
    // printf("\nPlayer 2 Move: ");
    //scanf("%s", input);
    struct fullDataTurn *choice = bot1Choice(main_board, 'B', status);
    if (choice == NULL)
    {
        *status = false;
        return;
    }
    printf("B is playing:\n");
    printMove(choice);
    buildFromHalfMove(main_board, choice, 'B', status);
    printDataBoard(main_board, true);
    free(choice);
}

void getPlayerWhiteMove(struct dataBoard *main_board, bool *status)
{
    printf("\nWhite Move: ");
    scanf("%s", input);
    struct fullDataTurn *choice = stringToFullDataTurn(main_board, input, 'W', status);
    buildFromHalfMove(main_board, choice, 'W', status);
    printf("W is playing:\n");
    printMove(choice);
    printDataBoard(main_board, true);
    free(choice);
}

void getPlayerBlackMove(struct dataBoard *main_board, bool *status)
{
    printf("\nBlack Move: ");
    scanf("%s", input);
    struct fullDataTurn *choice = stringToFullDataTurn(main_board, input, 'B', status);
    buildFromHalfMove(main_board, choice, 'B', status);
    printf("B is playing:\n");
    printMove(choice);
    printDataBoard(main_board, true);
    free(choice);
}

struct fullDataTurn *playerMove(struct dataBoard *main_board, char side, bool *status)
{
    printf("\n%c Move: ", side);
    scanf("%s", input);
    struct fullDataTurn *choice = stringToFullDataTurn(main_board, input, 'W', status);
    free(input);
    return choice;
}

void makeMove(struct dataBoard *main_board, char side, trnptr func_ptr)
{
    bool status = true;
    struct fullDataTurn *choice = func_ptr(main_board, side, &status);
    main_board = buildFromHalfMove(main_board, choice, side, &status);
    free(choice);
    assert(status);
}

trnptr getFlags(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            assert(argv[i][1] == 'b' || argv[i][1] == 'w');

            if (argv[i][1] == 'b')
            {
                if (strcmp(argv[i + 1], "player"))
                {
                    return &playerMove;
                }
                else if (strcmp(argv[i + 1], "bot1"))
                {
                    return &playerMove;
                }
                else if (strcmp(argv[i + 1], "bot2"))
                {
                    return &playerMove;
                }
                else if (strcmp(argv[i + 1], "bot3"))
                {
                    return &playerMove;
                }
                else if (strcmp(argv[i + 1], "bot4"))
                {
                    return &playerMove;
                }
                else
                {
                    assert(false);
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, signal_handler);
    srand(time(NULL));
    input = malloc(6);
    int move_number = 0;

    // if(argc == 1) {
    //     // visual mode
    //     printf("Visual Mode\n");
    //     return 0;
    // }

    // else if(argc != 4) {
    //     printf("Need Event Name\n");
    //     return 0;
    // }

    main_board = setupDataBoard();

    printDataBoard(main_board, true);

    trnptr func_ptr = getFlags(argc, argv);

    status = malloc(sizeof(int));
    *status = 0;
    while (move_number != -5)
    {
        move_number++;
        *status = 0;

        makeMove(main_board, 'W', func_ptr);

        checkForGameEnd(main_board, 'W');

        *status = 0;
        makeMove(main_board, 'B', func_ptr);
        checkForGameEnd(main_board, 'B');
        printf("move_number: %d\n", move_number);
    }

    free(status);
    free(main_board);

    return 0;
}