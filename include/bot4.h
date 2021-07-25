#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "standardlist.h"
#include "move.h"


struct boardMove {
    struct dataBoard *board;
    float score;
    struct fullDataTurn *turn;
};

void generateListofMoveForStartingMove(struct standardList *head, const char side, unsigned short depth_reached);
struct fullDataTurn *getBest(const struct dataBoard *input_board, const char side);

struct fullDataTurn *bot4Choice(const struct dataBoard *input_board, char side, bool *status);
