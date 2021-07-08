#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "move.h"

struct fullDataTurn* bot2Choice(const struct dataBoard* input_board, char side, bool* status);
struct basicDataTurnNode* getOnlyAttackingMoves(struct basicDataTurnNode* head, const struct dataBoard* input_board);