#include "piece-movements.h"

struct boardCheck *pawnMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken) {
    short pawnDir = side == 'W' ? 1 : -1;
    if (!attacking_if_taken) {
        if ((position->row == 1 && side == 'W') || (position->row == 6 && side == 'B')) { // for moving forward two steps if on row 2
            if (TeamOnSquare(input_board, position->row + 2*pawnDir, position->col) == ' ' && TeamOnSquare(input_board, position->row + 1*pawnDir, position->col) == ' ') { 
                validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 2*pawnDir, position->col));
            }
            else {
                // printf("test:%c:\n", TeamOnSquare(input_board, position->row + 2*pawnDir, position->col));
            }
        }
        if (TeamOnSquare(input_board, position->row + 1*pawnDir, position->col) == ' ') {
            validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col));
        }
    }
	//en passant tstuff
	if (side == 'W' && position->row == 4){
		// printf("piece to the right is of type: %d \n", getDataPiece(input_board, position->row, position->col + 1));
		// printf("piece to the right is of special %d \n", isDataPieceSpecial(getDataPiece(input_board, position->row, position->col + 1)));
		
		if ( validRange(position->col + 1) && getDataPiece(input_board, position->row, position->col + 1) == 13) {
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
		}
		else if(validRange(position->col - 1) && getDataPiece(input_board, position->row, position->col - 1) == 13 ) { // this is else ifbecause there is never more than one en passant possibility at a time
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
		}
	}

	if (side == 'B' && position->row == 3){
		if (validRange(position->col + 1) && getDataPiece(input_board, position->row, position->col + 1) == 12) {
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
		}
		else if(validRange(position->col - 1) && getDataPiece(input_board, position->row, position->col - 1) == 12) { // this is else ifbecause there is never more than one en passant possibility at a time
			validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
		}
	}
	//en passant stuff ends here


	// normal taking
	if (validRange(position->row + 1*pawnDir)) {
		if (attacking_if_taken || TeamOnSquare(input_board, position->row + 1*pawnDir, position->col - 1) == oppositeSide(side)) { // this
			if (validRange(position->row + 1*pawnDir) && validRange(position->col - 1)) {
				validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col - 1));
			}
		}

		if (attacking_if_taken || TeamOnSquare(input_board, position->row + 1*pawnDir, position->col + 1) == oppositeSide(side)) { // this
			if (validRange(position->row + 1*pawnDir) && validRange(position->col + 1)) {
				validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + 1*pawnDir, position->col + 1));
			}
		}
	}
	
	return validPositions;
}

struct boardCheck *knightMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken) {

    int multipliers[8][2] = {
    {2,  -1}, 
    {2,   1},
    {1,   2},
    {-1, -2},
    {-2,  1},
    {-2, -1},
    {-1,  2},
    {1,  -2}
    };

	for (int i = 0; i < 8; i++) {
		int new_row = position->row + multipliers[i][0];
		int new_col = position->col + multipliers[i][1];
		if (validRange(new_row)  && validRange(new_col)) {
			char combined[] = { ' ', oppositeSide(side)};

			if (attacking_if_taken || strchr(combined, pieceIdOfDataPiece(getDataPiece(input_board, new_row, new_col))) != NULL) {

				validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(new_row, new_col));
			}
		}
	}
	
	return validPositions;
}

struct boardCheck *bishopMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken) {
	int multipliers[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	char combined[] = { ' ', oppositeSide(side)};
	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1])) {
               if (strchr(combined, sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
					validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
					if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {

						break;	
					}
                }
				else if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == side) {

					break;
				}
			}
		}
	}
	
	return validPositions;
}

struct boardCheck *rookMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken) {
	int multipliers[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	char combined[] = { ' ', oppositeSide(side)};
	for (int i = 0; i < 4; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1])) {
                if (strchr(combined, sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
					validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
					if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {

						break;	
					}
                }
				else if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == side) {

					break;
				}
            }
		}
	}
	// printPosList(validPositions);
	return validPositions;
}

struct boardCheck *queenMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken) {
	
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	char combined[] = {' ', oppositeSide(side)};
	for (int i = 0; i < 8; i ++) {
		for (int j = 1; j < 8; j++) {
			if (validRange(position->row + j*multipliers[i][0]) && validRange(position->col + j*multipliers[i][1])) {
                if (strchr(combined, sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]))) != NULL || attacking_if_taken) {
					validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + j*multipliers[i][0], position->col + j*multipliers[i][1]));
					if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == oppositeSide(side)) {

						break;	
					}
                }
				else if (sideOfDataPiece(getDataPiece(input_board, position->row + j*multipliers[i][0], position->col + j*multipliers[i][1])) == side) {
					break;
				}
			}
		}
	}
	// printPosList(validPositions);
	return validPositions;
}


struct boardCheck *kingMovement(const struct dataBoard *input_board, struct boardCheck *validPositions, const struct standardPos *position, char side, bool attacking_if_taken) {
	int multipliers[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	struct standardPos *position_cpy = malloc(sizeof(struct standardPos));
	for (int i = 0; i < 8; i++) {
		position_cpy->row = position->row + multipliers[i][0];
		position_cpy->col = position->col + multipliers[i][1];
        if (validRange(position_cpy->row) && validRange(position_cpy->col)) {
            if (attacking_if_taken || sideOfDataPiece(getDataPiece(input_board, position_cpy->row, position_cpy->col)) != side) {
                validPositions->mask = setBitOfBoardCheck(validPositions, positionToIndex(position->row + multipliers[i][0], position->col + multipliers[i][1]));
            }	
        }
	}
	// printf("king\n");
	// printBoardCheck(validPositions);

	
	// printPosList(validPositions->next);
	free(position_cpy);

	return validPositions;
}