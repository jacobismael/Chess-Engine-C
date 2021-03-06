#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "standardlist.h"
#include "move.h"

struct basicDataTurnNode *getOnlyGoodAttackingMoves(struct basicDataTurnNode *head, const struct dataBoard *input_board);
struct fullDataTurn *bot3Choice(const struct dataBoard *input_board, char side, bool *status);
