#include "game.h"


unsigned char makeEmptyPiece() {
	return makeDataPiece(' ', ' ', false);
}

struct boardCheck *listOfLegalMoves(const struct dataBoard *input_board, const struct standardPos *position, const struct dataBoard *original_board, bool attacking_self) { //replace original board with a struct called board diff or smth
	struct boardCheck *validPositions = malloc(sizeof(struct boardCheck));
    validPositions->mask = 0;
	char side = sideOfDataPiece(getDataPiece(input_board, position->row, position->col));
	// printf("side: %c\n", side);
	// printf("position: %d %d\n", position->row, position->col);
	switch(pieceIdOfDataPiece(getDataPiece(input_board, position->row, position->col))) {
		case ' ':
			return NULL;
		case 'P':
			validPositions = pawnMovement(original_board, validPositions, position, side, attacking_self);
			break;
		case 'N':
			validPositions = knightMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'B':
			validPositions = bishopMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'R':
			validPositions = rookMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'Q':
			validPositions = queenMovement(input_board, validPositions, position, side, attacking_self);
			break;
		case 'K':
			validPositions = kingMovement(input_board, validPositions, position, side, attacking_self);
			break;
	}
    // printf("result: %ld\n", validPositions->mask);
	return validPositions;
}

struct dataBoard *buildFromStart(struct dataBoard *input_board, struct Move *head, bool *status) {

	while (head != NULL) {
		input_board = buildFromMove(input_board, head, status);
		printDataBoard(input_board, true);
		
		head = head->next;
	}
	return input_board;
}


struct dataBoard *buildFromMove(struct dataBoard *input_board, struct Move *move, bool *status) {
	//white's move
	struct fullDataTurn *choice = stringToFullDataTurn(input_board, move->white_notation, 'W', status);
	input_board = buildFromHalfMove(input_board, choice, 'W', NULL);
	free(choice);
	choice = stringToFullDataTurn(input_board, move->white_notation, 'W', status);
	input_board = buildFromHalfMove(input_board, choice, 'B', NULL);
	free(choice);

	return input_board;
}

struct dataBoard *castleHandling(struct dataBoard *input_board, struct fullDataTurn *truemove, char side, bool *status) {
	*status = false;
    if (kingInCheck(input_board, side)) {
        return input_board;
    }
    int end_row = side == 'W' ? 0 : 7;
    unsigned char blank_piece = makeEmptyPiece();
    unsigned char king_piece = makeDataPiece('K', side, true);
    unsigned char rook_piece = makeDataPiece('R', side, true);
    if (truemove->is_king_side) {
        if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 5)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 6)) == ' ') { // checks that the squares between the king are empty
            
            struct standardPos positions_to_eval[2] = {
                {.row = end_row, .col = 5},
                {.row = end_row, .col = 6}    
            };
            // we dont need to check the king's square because then it would be in check and we have already checked for that
            if (!positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[0])) && !positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[1]))) { 
                if(pieceIdOfDataPiece(getDataPiece(input_board, end_row, 4)) == 'K' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 7)) == 'R') {
					if (!isDataPieceSpecial(getDataPiece(input_board, end_row, 4)) && !isDataPieceSpecial(getDataPiece(input_board, end_row, 7))) {
						*status = true;
						
						setDataPiece(input_board, end_row, 4, blank_piece);
						setDataPiece(input_board, end_row, 5, rook_piece);
						setDataPiece(input_board, end_row, 6, king_piece);
						setDataPiece(input_board, end_row, 7, blank_piece);
					}
                }
            }
        }
    }
    else {
        if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 1)) == ' ' &&  pieceIdOfDataPiece(getDataPiece(input_board, end_row, 2)) == ' '  && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 3)) == ' ') { // checks that the squares between the king are empty
            // printf("here\n");
            struct standardPos positions_to_eval[2] = {
                {.row = end_row, .col = 2},
                {.row = end_row, .col = 3}    
            };
            if (!positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[0])) && !positionUnderAttack(input_board, oppositeSide(side), &(positions_to_eval[1]))) { 
                if(pieceIdOfDataPiece(getDataPiece(input_board, end_row, 4)) == 'K' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 0)) == 'R') {
					if (!isDataPieceSpecial(getDataPiece(input_board, end_row, 4)) && !isDataPieceSpecial(getDataPiece(input_board, end_row, 0))) {
						*status = true;
						
						setDataPiece(input_board, end_row, 0, blank_piece);
						setDataPiece(input_board, end_row, 1, blank_piece);
						setDataPiece(input_board, end_row, 3, rook_piece);
						setDataPiece(input_board, end_row, 2, king_piece);
						setDataPiece(input_board, end_row, 4, blank_piece);
					}
                }
            }
        }
    }
    return input_board;
}

struct dataBoard *removeEnPassants(struct dataBoard *input_board, char side) {
	
	for (int i = 0; i < 8; i++ ) {
		for (int j = 0; j < 8; j++ ) {
			if (side == 'W' && getDataPiece(input_board, i, j) == 12) {
				setDataPiece(input_board, i, j, 0);
			}
			else if (side == 'B' && getDataPiece(input_board, i, j) == 13) {
				setDataPiece(input_board, i, j, 6);
			}
		}	
	}

	return input_board;
}

struct fullDataTurn *toFullDataTurn(struct dataTurn *input_turn, const struct dataBoard *input_board, char side, bool *status) {
	struct fullDataTurn *final = malloc(sizeof(struct fullDataTurn));
	final->is_special = false;
	*status = 0;
	final->castles = input_turn->castles;
	final->is_king_side = input_turn->is_king_side;
	final->is_check = input_turn->is_check;
	if (final->castles) {
		*status = 1;
		return final;
	}

	final->final_position.row = input_turn->final_position.row - 1, 
	final->final_position.col = input_turn->final_position.col;
	assert(validRange(final->final_position.row));
	assert(validRange(final->final_position.col));
	final->piece_promotes_to = input_turn->piece_promotes_to;
	if(final->piece_promotes_to == 'K') {// you cannot promote to a king
		*status = 0;
		printf("you cannot promot to a king");
		return final;

	}
	final->piece = input_turn->piece;
	final->takes = input_turn->takes;


	//en passant
	final->is_en_passant = false; 
	if (final->takes && sideOfDataPiece(getDataPiece(input_board, final->final_position.row , final->final_position.col)) == ' ') {
		if (isDataPieceSpecial(getDataPiece(input_board, final->final_position.row + (side == 'W' ? -1 : 1), final->final_position.col))) {
			final->is_en_passant = true;
		}
	}


	bool is_piece_special = false;
	assert(side == 'W' || side == 'B');
	
	

	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));
	memcpy(copy_board, input_board, sizeof(struct dataBoard)); //this feels inefficient instead maybe save specific positions and 

	
	if (input_turn->takes && validRange(input_turn->final_position.row - 1)) {
		if (sideOfDataPiece(getDataPiece(copy_board, input_turn->final_position.row -1, input_turn->final_position.col)) == oppositeSide(side)) {
			// adds replaced peice to list of pieces that are off the board
			if (strchr("K ",  pieceIdOfDataPiece(getDataPiece(copy_board, input_turn->final_position.row -1, input_turn->final_position.col)) == '\0')) { 
				setDataPiece(copy_board, input_turn->final_position.row -1, input_turn->final_position.col, 31);
			}
		}
	}

	struct standardPos temp_position = {.row = 0, .col = 0};
	struct boardCheck *lolm = NULL;
	struct standardPos expected_result = {.row = input_turn->final_position.row -1, .col = input_turn->final_position.col};
	struct standardPos restrictors = input_turn->restrictors;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((restrictors.row == i || restrictors.row == -1) && (restrictors.col == j || restrictors.col == -1)) { 
			// this is The line to focus on
			//also add a game.c with a game struct etc 
				if (pieceIdOfDataPiece(getDataPiece(copy_board, i, j)) == input_turn->piece && sideOfDataPiece(getDataPiece(copy_board, i, j)) == side) {
					temp_position.row = i;
					temp_position.col = j; 

					lolm = listOfLegalMoves(copy_board, &temp_position, input_board, false);
                    // printf("lolm: %ld\n", lolm->mask);

					if (getBitOfBoardCheck(lolm, positionToIndex(expected_result.row, expected_result.col))) {
                        // printBoardCheck(lolm);
						//checks if piece can be en passanted in the future
						if (!is_piece_special) {
							is_piece_special = isDataPieceSpecial(getDataPiece(copy_board, i, j));
						}
						if (!is_piece_special && input_turn->piece == 'P' ) {
							if(side == 'W') {
								if (expected_result.row == 3 && i == 1) {
									is_piece_special = true;
								}
							}
							else if(side == 'B') {
								if (expected_result.row == 4 && i == 6) {
									is_piece_special = true;
								}
							}
						}
						setDataPiece(copy_board, i, j, 31);
						*status = 1;
						final->starting_position = temp_position;
                        free(lolm);
						break;
					}
                    free(lolm);
				}
			}
		}
	}

	//prep for en passant
	if (side == 'W' && pieceIdOfDataPiece(getDataPiece(input_board, final->final_position.row, final->final_position.col) == 'P')) {
		if (final->final_position.row == 3 && final->starting_position.row == 1) {
			final->is_special = true;
		}
	}
	if (side == 'B' && pieceIdOfDataPiece(getDataPiece(input_board, final->final_position.row, final->final_position.col) == 'P')) {
		if (final->final_position.row == 4 && final->starting_position.row == 6) {
			final->is_special = true;
		}
	}

	if (final->piece == 'K' || final->piece == 'R') {
		final->is_special = true;
	}

	// assert(status == 1);

	free(copy_board);
	return final;
}

bool isMate(const struct dataBoard *input_board, char side) {
	//this doesnt support en passant yet

	if (!kingInCheck(input_board, side)) {
		return false;
	}

	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));
	memcpy(copy_board, input_board, sizeof(struct dataBoard));

	unsigned char temp_piece = ' ';
	struct standardPos temp_pos; 
	struct boardCheck *lolm = NULL;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (validRange(i /* is this right? */) &&  sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
				temp_pos.row = i;
				temp_pos.col = j;
				
				// printf("piece: %c\n", pieceIdOfDataPiece(getDataPiece(input_board, temp_pos.row, temp_pos.col)));
				lolm = listOfLegalMoves(input_board, &temp_pos, copy_board, false);
				// printBoardCheck(lolm);
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						memcpy(copy_board, input_board, sizeof(struct dataBoard));
						if(getBitOfBoardCheck(lolm, positionToIndex(k, l))) {
							temp_piece = getDataPiece(copy_board, temp_pos.row, temp_pos.col);
							setDataPiece(copy_board, temp_pos.row, temp_pos.col, 31);
							setDataPiece(copy_board, k, l, temp_piece);
							// printDataBoard(copy_board);

							if (!kingInCheck(copy_board, side)) {
								free(lolm);
								free(copy_board);
								return false;
							}
						}
					}
				}
				free(lolm);
			}
		}
	}
	free(copy_board);
	return true;

}

extern inline int randomInt(int min, int max) { // stolen from stack overflow user= sleutheye
   return min + rand() % ((max - min) > 0 ? (max - min) : 1);
}


struct standardList *allBasicLegalMoves(const struct dataBoard *input_board, char side)  {
	struct standardList *head = NULL;
	struct standardPos temp_start_pos = {.row = 0, .col = 0};
	struct standardPos temp_end_pos = {.row = 0, .col = 0};
	struct boardCheck *lolm;
	struct dataBoard *copy_board = malloc(sizeof(struct dataBoard));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (sideOfDataPiece(getDataPiece(input_board, i , j)) == side) {
				temp_start_pos.row = i;
				temp_start_pos.col = j;


				lolm = listOfLegalMoves(input_board, &temp_start_pos, input_board, false);
				if (lolm != NULL) {
					// printBoardCheck(lolm);
					for (int k = 0; k < 8; k++) {
						for (int l = 0; l < 8; l++) {
							if (getBitOfBoardCheck(lolm, positionToIndex(k, l)) && pieceIdOfDataPiece(getDataPiece(input_board, l, k)) != 'K') {
								memcpy(copy_board, input_board, sizeof(struct dataBoard));
								temp_end_pos.row = k;
								temp_end_pos.col = l;
								
								setDataPiece(copy_board, i, j, 31);
								setDataPiece(copy_board, k, l, getDataPiece(input_board, i, j));
								if (!kingInCheck(copy_board, side)) {

									struct basicDataTurn *basic = malloc(sizeof(struct basicDataTurn));
									basic->starting_pos = temp_start_pos;
									basic->ending_pos = temp_end_pos;
									head = prependToStandardList(head, basic);
								}
							}
						}
					}
					free(lolm);
				}
			}
		}
	}
	free(copy_board);
	return head;
}

struct standardList *allLegalMoves(const struct dataBoard *input_board, char side) {
	struct standardList *head = NULL;

	if (canCastle(input_board, side, true)) {
		struct fullDataTurn *new = malloc(sizeof(struct fullDataTurn)); 
		new->final_position.row = 0;
		new->final_position.col = 0;
		new->starting_position.row = 0;
        new->starting_position.col = 0;
		new->piece = ' ';
		new->is_en_passant = false;
		new->is_special = false;
		new->takes = false;
		new->castles = true;
		new->is_king_side = true;
		new->piece_promotes_to = ' ';
		head = prependToStandardList(head, new);
	}
	if (canCastle(input_board, side, false)) { // it can be both king and queen side
		struct fullDataTurn *new = malloc(sizeof(struct fullDataTurn)); 
		new->final_position.row = 0;
		new->final_position.col = 0;
		new->starting_position.row = 0;
        new->starting_position.col = 0;
		new->piece = ' ';   
		new->is_en_passant = false;
		new->is_special = false;
		new->takes = false;
		new->castles = true;
		new->is_king_side = true;
		new->piece_promotes_to = ' ';
		head = prependToStandardList(head, new);
	}
	
	struct standardList *basic_move_head = allBasicLegalMoves(input_board, side);
	if (basic_move_head == NULL) {
		return NULL;
	}
	struct standardList *basic_move_head_cpy = basic_move_head;
	while (basic_move_head->next != NULL) {
		struct fullDataTurn *new = malloc(sizeof(struct fullDataTurn));
		new->castles = false;
		new->final_position = ((struct basicDataTurn*)(basic_move_head->data))->ending_pos;
		// printf("ending_pos = %d, %d\n", new->final_position.row, new->final_position.col);
		new->starting_position = ((struct basicDataTurn*)(basic_move_head->data))->starting_pos;
		new->piece = pieceIdOfDataPiece(getDataPiece(input_board, new->starting_position.row, new->starting_position.col));
		//en passant
		new->is_en_passant = false; 
		if (new->piece == 'P' && (new->final_position.row + (side == 'W' ? -1 : 1))) {
			if (sideOfDataPiece(getDataPiece(input_board, new->final_position.row , new->final_position.col)) == ' ') {
				if (isDataPieceSpecial(getDataPiece(input_board, new->final_position.row + (side == 'W' ? -1 : 1), new->final_position.col))) {
					if (pieceIdOfDataPiece(getDataPiece(input_board, new->final_position.row + (side == 'W' ? -1 : 1), new->final_position.col))) {
						new->is_en_passant = true;
					}
				}
			}
		}
		//prep for en passant next move

		new->is_special = false;
		if (side == 'W' && pieceIdOfDataPiece(getDataPiece(input_board, new->starting_position.row, new->starting_position.col) == 'P')) {
			if (new->final_position.row == 3 && new->starting_position.row == 1) {
				new->is_special = true;
			}
		}
		if (side == 'B' && pieceIdOfDataPiece(getDataPiece(input_board, new->starting_position.row, new->starting_position.col) == 'P')) {
			if (new->final_position.row == 4 && new->starting_position.row == 6) {
				new->is_special = true;
			}
		}
		if (new->piece == 'K' || new->piece == 'R') {
			new->is_special = true;
		}
		if (new->piece != 'K' && new->piece != 'R' && new->piece != 'P') {
			new->is_special = false;
		}
		new->takes = sideOfDataPiece(getDataPiece(input_board, new->final_position.row, new->final_position.col)) == oppositeSide(side);
		new->is_king_side = false;

		new->piece_promotes_to = ' ';
		if (pieceIdOfDataPiece(getDataPiece(input_board, new->final_position.row, new->final_position.col)) != 'K') {
			head = prependToStandardList(head, new);
		}
		else {
			printf("oh no\n");
		}
		// printf("ending_pos2 %d\n", ((struct fullDataTurn*)he;d->data)->final_position.row);
		// printf("ending_pos again = %d, %d\n"q, );
		// printf("ending_pos again = %d, %d\n", new->final_position.row, new->final_position.col);
		basic_move_head = basic_move_head->next;
		// printf("here %p", head);
	}
	freeStandardList(basic_move_head_cpy);
	
	return head;
}

bool isDraw(const struct dataBoard *input_board, char side) { // very simple and incomplete still
	//this whole function doesnt support en passant yet
	if (kingExists(input_board, side) && kingInCheck(input_board, side)) {
		return false;
	}
	struct standardList *head;
	head = allBasicLegalMoves(input_board, side);
	if (head == NULL) {
		freeStandardList(head);
		return true;
	}
	
	// 0 1 2 3 4 5
	// K Q B N R P
	int num_each_piece_w[6] = {0};
	int num_each_piece_b[6] = {0};
	int position = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (pieceIdOfDataPiece(getDataPiece(input_board, i, j))) {
				case 'K':
					position = 0;
					break;
				case 'Q':
					position = 1;
					break;
				case 'B':
					position = 2;
					break;
				case 'N':
					position = 3;
					break;
				case 'R':
					position = 4;
					break;
				case 'P':
					position = 5;
					break;					
			}
			if (sideOfDataPiece(getDataPiece(input_board, i, j)) == 'W') {
				num_each_piece_w[position]++;
			}
			else if (sideOfDataPiece(getDataPiece(input_board, i, j)) == 'B') {
				num_each_piece_b[position]++;
			}
		}	
	}
	freeStandardList(head);
	//cases where it isnt a draw
	if (num_each_piece_b[5] + num_each_piece_w[5] >= 1) { //any pawns left
		return false;
	}
	if (num_each_piece_b[1] + num_each_piece_w[1] >= 1) { //any pawns left
		return false;
	}
	if ((num_each_piece_b[2] >= 1 && num_each_piece_b[3] >= 1) && (num_each_piece_w[2] >= 1 && num_each_piece_w[3] >= 1)) {
		return false;
	}
	if (num_each_piece_b[4] + num_each_piece_w[4] >= 1) {
		return false;
	}
	return true;

}


struct fullDataTurn *stringToFullDataTurn(const struct dataBoard *input_board, char *turn, char side, bool *status) {
	struct dataTurn *cmove = toDataTurn(turn);

	if (cmove == NULL) {
		printf("Illegal move sent\n");
		*status = 0;
		return NULL;
	}

	struct fullDataTurn *truemove = toFullDataTurn(cmove, input_board, side, status);
	free(cmove);
	return truemove;
}


void moveIsValid(struct dataBoard *input_board, struct fullDataTurn *truemove) { // this function should be more rigorous and larger
	if (truemove->takes) {
		assert(pieceIdOfDataPiece(getDataPiece(input_board, truemove->final_position.row, truemove->final_position.col)) != ' ');
	}
	else {
		assert(pieceIdOfDataPiece(getDataPiece(input_board, truemove->final_position.row, truemove->final_position.col)) == ' ');
	}
}

struct dataBoard *buildFromHalfMove(struct dataBoard *input_board, struct fullDataTurn *truemove, char side, bool *status) {
	if (!*status) {
		return input_board;
	}
	//moveIsValid(input_board, truemove);
	struct dataBoard copy_board = *input_board;
	//castle handling
	if (truemove->castles) {
		printf("in castles\n");
		return castleHandling(input_board, truemove, side, status);
    	}
	//remove pieces marked as being able to be enpassanted with
	input_board = removeEnPassants(input_board, side); // remove en passants for own side from previous move
	setDataPiece(input_board, truemove->starting_position.row, truemove->starting_position.col, makeEmptyPiece());

	//assigns special pieces
	if (*status) {
		setDataPiece(input_board, truemove->final_position.row, truemove->final_position.col, makeDataPiece(truemove->piece, side, truemove->is_special));
	}
	//en passant remove piece that was taken
	if (truemove->is_en_passant) {
		assert(truemove->piece == 'P');
		setDataPiece(input_board, truemove->final_position.row + (side == 'W' ? -1 : 1), truemove->final_position.col, makeEmptyPiece());
	}
	//promotion
	if (truemove->piece =='P') {
		if (truemove->final_position.row == 7 && side == 'W') {
			// it defaults to queen if nothing is specified
			setDataPiece(input_board, truemove->final_position.row, truemove->final_position.col, (truemove->piece_promotes_to != ' ' ? 
			                                                                                       makeDataPiece(truemove->piece_promotes_to, side, false) : 
			                                                                                       makeDataPiece('Q', side, false)));
		}
		if (truemove->final_position.row == 0 && side == 'B') {
			// it defaults to queen if nothing is specified
			setDataPiece(input_board, truemove->final_position.row, truemove->final_position.col, (truemove->piece_promotes_to != ' ' ?  
			                                                                                       makeDataPiece(truemove->piece_promotes_to, side, false) : 
																					               makeDataPiece('Q', side, false)));
		}
	}

	if (!*status) {
		*input_board = copy_board;
	}
	// free(truemove);
	return input_board;
}

bool positionUnderAttack(const struct dataBoard *input_board, char attacking_side, const struct standardPos *position) { // this is probably broken
    assert(attacking_side == 'W' || attacking_side == 'B');

    struct boardCheck *lolm = malloc(sizeof(struct boardCheck));
    lolm->mask = 0;
    
	struct boardCheck *temp = malloc(sizeof(struct boardCheck));
    free(temp);
	struct dataBoard *pawn_board = malloc(sizeof(struct dataBoard));

	memcpy(pawn_board, input_board, sizeof(struct dataBoard));
	if (validRange(position->row) && validRange(position->col)) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (sideOfDataPiece(getDataPiece(input_board, i, j)) == attacking_side) {
					setDataPiece(pawn_board, position->row, position->col, makeDataPiece('K', oppositeSide(attacking_side), true)); // the last bool can be true or false in this situation 
					struct standardPos *temp_pos = malloc(sizeof(struct standardPos));
					temp_pos->row = i;
					temp_pos->col = j;
					temp = listOfLegalMoves(input_board, temp_pos, pawn_board, true);
                    lolm->mask |= temp->mask;
                    free(temp);
					free(temp_pos);
				}	
			}	
		}
	}
    // printBoardCheck(lolm);
    if (getBitOfBoardCheck(lolm, positionToIndex(position->row, position->col))) {
        free(lolm);   
        free(pawn_board);
        return true;
    }
    free(lolm);
	free(pawn_board);
	return false;
} 

bool kingInCheck(const struct dataBoard *input_board, char side) {
	assert(side == 'W' || side == 'B');
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
            if (pieceIdOfDataPiece(getDataPiece(input_board, i, j)) == 'K') {
			    if (sideOfDataPiece(getDataPiece(input_board, i, j)) == side) {
                    // printf("king is at %d %d\n", i, j);
					struct standardPos temp_pos = {.row = i, .col = j};
					bool x = positionUnderAttack(input_board, oppositeSide(side), &temp_pos);
                    // printf( "is king under attack: %d", x);
                    return x;
                }
			}
		}
	}
	//assert(false); //this means there is no king on the board which shouldnt happen

}


bool canCastle(const struct dataBoard *input_board, char side, bool is_king_side) {
	//it prefers to castle king side
	unsigned char end_row = side == 'W' ? 0 : 7;
	//i thcecks for king side first
	if (kingInCheck(input_board, side)) {
		return false; // you cant castle when in check
	}
	if (is_king_side) {
		if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 5)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 6)) == ' ') { // checks squares are empty
			if (getDataPiece(input_board, end_row, 4) == makeDataPiece('K', side, false) && getDataPiece(input_board, end_row, 7) == makeDataPiece('R', side, false)) {
					struct standardPos temp_positions[2] = {{.row = end_row, .col = 5}, {.row = end_row, .col = 6}};
					if (!positionUnderAttack(input_board, oppositeSide(side), &temp_positions[0]) && positionUnderAttack(input_board, oppositeSide(side), &temp_positions[1])) {
						return true;
					}
			}
		}
		return false;
	}
	else {
		if (pieceIdOfDataPiece(getDataPiece(input_board, end_row, 1)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 2)) == ' ' && pieceIdOfDataPiece(getDataPiece(input_board, end_row, 3)) == ' ') { // checks squares are empty
			if (getDataPiece(input_board, end_row, 0) == makeDataPiece('R', side, false) && getDataPiece(input_board, end_row, 4) == makeDataPiece('K', side, false)) {
					struct standardPos temp_positions[2] = {{.row = end_row, .col = 2}, {.row = end_row, .col = 3}};
					if (!positionUnderAttack(input_board, oppositeSide(side), &temp_positions[0]) && positionUnderAttack(input_board, oppositeSide(side), &temp_positions[1])) {
						return true;
					}
			}
		}
		return false;
	}
}

float getBoardScore(const struct dataBoard *input_board) {
	if(isMate(input_board, 'B')) {
		return -INFINITY;
	}
	if(isMate(input_board, 'W')) {
		return INFINITY;
	}

	float score = getBasicBoardScore(input_board);
	if (kingInCheck(input_board, 'B')) {
		score += 0.5;
	}
	if (kingInCheck(input_board, 'W')) {
		score -= 0.5;
	}
	return score;
}