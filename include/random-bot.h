#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "move.h"

struct basicDataTurnNode* allBasicLegalMoves(const struct dataBoard* input_board, char side) ;
struct fullDataTurn* randomChoice(const struct dataBoard* input_board, char side, bool* status);