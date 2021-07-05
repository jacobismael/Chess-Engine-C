#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "board.h"
#include "move.h"


struct boardCheck* pawnMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken);
struct boardCheck* knightMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken);
struct boardCheck* bishopMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken);
struct boardCheck* rookMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken);
struct boardCheck* queenMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken);
struct boardCheck* kingMovement(const struct dataBoard* input_board, struct boardCheck* validPositions, const struct standard_pos* position, char side, bool attacking_if_taken);
struct boardCheck* listOfLegalMoves(const struct dataBoard* input_board, const struct standard_pos* position, const struct dataBoard* original_board, bool attacking_self);

struct dataBoard* buildFromStart(struct dataBoard* input_board, struct Move* head);
struct dataBoard* buildFromMove(struct dataBoard* input_board, struct Move* move);
struct dataBoard* buildFromHalfMove(struct dataBoard* input_board, char* move, char side, bool* status);

struct dataBoard* castleHandling(struct dataBoard* input_board, struct dataTurn* cmove, char side, bool* status);
struct dataBoard* removeEnPassants(struct dataBoard* input_board, char side);
struct fullDataTurn* toFullDataTurn(struct dataTurn* input_turn, struct dataBoard* input_board, char side, bool *is_special, bool *status);
bool isMate(struct dataBoard* input_board, char side);



bool positionUnderAttack(const struct dataBoard* input_board, char attacking_side, const struct standard_pos* position);
bool kingInCheck(const struct dataBoard* input_board, char side);
