#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgn-functions.h"

struct Result *MakeResult(float w_score, float b_score) {
    struct Result *result = malloc(sizeof(struct Result));

    result->white_score = w_score;
    result->black_score = b_score;

    return result;
}

void PrintResult(struct Result *result) {

    char *white_str = malloc(2);
    char *black_str = malloc(2);

    sprintf(white_str, "%d", (short)result->white_score);
    sprintf(black_str, "%d", (short)result->black_score);

    printf("%s-%s", result->white_score == 0.5 ? "1/2" : white_str, result->black_score == 0.5 ? "1/2" : black_str);

    free(white_str);
    free(black_str);
}

char* getResult(struct Result *result) {

    char* output = malloc(1000);
    char* buff = malloc(200);

    char *white_str = malloc(2);
    char *black_str = malloc(2);

    sprintf(white_str, "%d", (short)result->white_score);
    sprintf(black_str, "%d", (short)result->black_score);

    sprintf(buff, "%s-%s", result->white_score == 0.5 ? "1/2" : white_str, result->black_score == 0.5 ? "1/2" : black_str);
    strncat(output, buff, strlen(buff));

    free(white_str);
    free(black_str);
    free(buff);

    return output;
}

void PrintGame(struct PGN *game) {
    printf("\n===================| %s |===================\n", game->event);
    
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

void PrintGameStrict(struct PGN *game) {
    printf("[Event \"%s\"]\n", game->event);
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

void SavePGNtoFile(struct PGN *game, char* filepath) {
    FILE *fp;

    char* output = malloc(1000);
    char* buff = malloc(200);
    char* path = malloc(200);

    sprintf(buff, "[Event \"%s\"]\n", game->event);
    strncat(output, buff, strlen(buff));
    sprintf(buff, "[Site \"%s\"]\n", game->site);
    strncat(output, buff, strlen(buff));
    sprintf(buff, "[Date \"%s\"]\n", game->date);
    strncat(output, buff, strlen(buff));
    sprintf(buff, "[Round \"%d\"]\n", game->round);
    strncat(output, buff, strlen(buff));
    sprintf(buff, "[White \"%s\"]\n", game->white);
    strncat(output, buff, strlen(buff));
    sprintf(buff, "[Black \"%s\"]\n", game->black);
    strncat(output, buff, strlen(buff));

    sprintf(buff, "[Result \"");
    strncat(output, buff, strlen(buff));
    sprintf(buff, "%s", getResult(game->result));
    strncat(output, buff, strlen(buff));
    sprintf(buff, "\"]\n\n");
    strncat(output, buff, strlen(buff));

    sprintf(buff, "%s", getMoveList(game->move_head));
    strncat(output, buff, strlen(buff));
    sprintf(buff, "%s", getResult(game->result));
    strncat(output, buff, strlen(buff));
    sprintf(buff, "\n");
    strncat(output, buff, strlen(buff));

    strncat(filepath, ".pgn", 5);
    
    sprintf(path, "../games/");
    strncat(path, filepath, strlen(filepath));


    printf("Writing PGN Notation to %s\n", path);


    fp = fopen(path, "w+");
    fprintf(fp, "%s", output);
    fclose(fp);
}
