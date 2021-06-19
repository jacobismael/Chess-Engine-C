#include <stdio.h>
#include <stdlib.h>

#include "pgn-functions.h"

struct Result* MakeResult(float w_score, float b_score) {
    struct Result* result = malloc(sizeof(struct Result));

    result->white_score = w_score;
    result->black_score = b_score;

    return result;
}

void PrintResult(struct Result* result) {

    char* white_str = malloc(2);
    char* black_str = malloc(2);

    snprintf(white_str, 2, "%f", result->white_score);
    snprintf(black_str, 2, "%f", result->black_score);

    printf("%s-%s", result->white_score == 0.5 ? "1/2" : white_str, result->black_score == 0.5 ? "1/2" : black_str);

    free(white_str);
    free(black_str);
}

void PrintGame(struct PGN* game) {
    printf("\n===================| %s |===================\n", game->event);;
    printf("Site: %s\n", game->site);
    printf("Date: %s\n", game->date);
    printf("Round: %d\n", game->round);
    printf("White: %s\n", game->white);
    printf("Black: %s\n", game->black);
    
    PrintResult(game->result);

    printf("\n===================| Moves |===================\n");

    PrintMoveList(game->move_head);
    printf("\n");
}


void PrintGameStrict(struct PGN* game) {
    printf("[Event \"%s\"]\n", game->event);;
    printf("[Site \"%s\"]\n", game->site);
    printf("[Date \"%s\"]\n", game->date);
    printf("[Round \"%d\"]\n", game->round);
    printf("[White \"%s\"]\n", game->white);
    printf("[Black \"%s\"]\n", game->black);

    printf("[Result \"");
    PrintResult(game->result);
    printf("\"]\n\n");

    PrintMoveList(game->move_head);
    PrintResult(game->result);
    printf("\n");
}
