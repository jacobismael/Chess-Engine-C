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

unsigned char makeEmptyPiece();
struct boardCheck *listOfLegalMoves(const struct dataBoard *input_board, const struct standardPos *position, const struct dataBoard *original_board, bool attacking_self);
struct dataBoard *buildFromStart(struct dataBoard *input_board, struct Move *head, bool *status);
struct dataBoard *buildFromMove(struct dataBoard *input_board, struct Move *move, bool *status);
struct dataBoard *buildFromHalfMove(struct dataBoard *input_board, struct fullDataTurn *truemove, char side, bool *status);

struct fullDataTurn *stringToFullDataTurn(const struct dataBoard *input_board, char *turn, char side, bool *status);
struct fullDataTurn *toFullDataTurn(struct dataTurn *input_turn, const struct dataBoard *input_board, char side, bool *status);
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

float getBoardScore(const struct dataBoard *input_board);
#endif 
