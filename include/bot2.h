#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "move.h"

struct fullDataTurn* bot2Choice(const struct dataBoard* input_board, char side, bool* status);
struct basicDataTurnNode* getOnlyAttackingMoves(struct basicDataTurnNode* head, const struct dataBoard* input_board);
bool doesTake(const struct dataBoard* input_board, struct standard_pos* starting_position, struct standard_pos* final_position);