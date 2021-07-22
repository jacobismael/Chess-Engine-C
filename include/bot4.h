#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "standardlist.h"
#include "move.h"

struct boardNode {
    struct boardNode *next;
    struct boardNode *child;
    struct dataBoard *board;
    struct fullDataTurn *turn;
    float score;
};

void freeTree(struct boardNode *head);
float bestMinScore(struct boardNode* head, bool aim_high);
struct fullDataTurn* bestNode(struct boardNode *head, const char side);
struct boardNode *generateTree(struct dataBoard* input_board, const char side, unsigned short depth);
void buildOnTree(struct boardNode *input_node, char side, unsigned short current_iteration);
void addLayer(struct boardNode *input_node, const char side);
struct fullDataTurn *bot4Choice(struct dataBoard *input_board, char side, bool *status);
