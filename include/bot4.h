#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "standardlist.h"
#include "move.h"


struct boardMove {
    struct dataBoard board;
    float score;
    struct fullDataTurn turn;
};

struct branch {
    struct branch *child;
    struct branch *parent;
    struct branch *next;
    struct boardMove data;
};


void addToTree(struct branch *head, const struct dataBoard *input_board, const char side, unsigned char depth_reached);
struct branch *generateTree(const struct dataBoard *input_board, const char side);
void prependToChildren(struct branch *input_branch, struct boardMove new_data);
struct fullDataTurn *getBest(const struct branch *head, const char starting_side);
float getBestScoreOfBranch(const struct branch *head, bool aim_high);