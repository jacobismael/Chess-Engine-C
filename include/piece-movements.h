#ifndef PIECE_MOVEMENTS_H
#define PIECE_MOVEMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"


struct boardCheck *pawnMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken);
struct boardCheck *knightMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken);
struct boardCheck *bishopMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken);
struct boardCheck *rookMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken);
struct boardCheck *queenMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken);
struct boardCheck *kingMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken);
struct boardCheck *listOfLegalMoves(const struct dataBoard *input_board, const struct standardPos *position, const struct dataBoard *original_board, bool attacking_self);

#endif