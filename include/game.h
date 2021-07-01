#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "move.h"

struct dataBoard* buildFromStart(struct dataBoard* input_board, struct Move* head);
struct dataBoard* buildFromMove(struct dataBoard* input_board, struct Move* move);
struct dataBoard* buildFromHalfMove(struct dataBoard* input_board, char* move, char side, int* status);

struct pos* pawnMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side);
struct pos* knightMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side);
struct pos* bishopMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side);
struct pos* rookMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side);
struct pos* queenMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side);
struct pos* kingMovement(const struct dataBoard* input_board, struct pos* validPositions, const struct standard_pos* position, char side);

bool positionUnderAttack(const struct dataBoard* input_board, char attacking_side, const struct standard_pos* position);
bool kingInCheck(const struct dataBoard* input_board, char side);
