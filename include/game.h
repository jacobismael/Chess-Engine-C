#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "piece-movements.h"
#include "standardlist.h"
#include "board.h"
#include "move.h"

struct dataBoard *buildFromStart(struct dataBoard *input_board, struct Move *head);
struct dataBoard *buildFromMove(struct dataBoard *input_board, struct Move *move);
struct dataBoard *buildFromHalfMove(struct dataBoard *input_board, struct fullDataTurn *truemove, char side, bool *status);

struct fullDataTurn *stringToFullDataTurn(struct dataBoard *input_board, char *turn, char side, bool *status);
extern inline unsigned char makeEmptyPiece();
struct dataBoard *castleHandling(struct dataBoard *input_board, struct fullDataTurn *cmove, char side, bool *status);
struct dataBoard *removeEnPassants(struct dataBoard *input_board, char side);
bool isMate(const struct dataBoard *input_board, char side);
bool isDraw(const struct dataBoard *input_board, char side);

struct standardList *allBasicLegalMoves(const struct dataBoard *input_board, char side);
struct standardList *allLegalMoves(const struct dataBoard *input_board, char side);
int randomInt(int min, int max);

bool positionUnderAttack(const struct dataBoard *input_board, char attacking_side, const struct standardPos *position);
bool kingInCheck(const struct dataBoard *input_board, char side);
bool canCastle(const struct dataBoard *input_board, char side, bool is_king_side);

#endif 
