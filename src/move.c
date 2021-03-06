#include "move.h"

struct Move *AddMove(struct Move *head, char *w_notation, char *b_notation) {
    struct Move *head_cpy = head;
    int position = 2;
    while(head->next != NULL) {
        head = head->next;
        position++;
    }
    struct Move *new_move = malloc(sizeof(struct Move));
    new_move->next = NULL;
    strncpy(new_move->white_notation , w_notation, strlen(w_notation));
    strncpy(new_move->black_notation , b_notation, strlen(b_notation));
    new_move->move_number = position;

    head->next = new_move;

    return head_cpy;
}


void printMove(const struct fullDataTurn *node) {
	printf("best:\n");
	printf("castles %d\n", node->castles);
	printf("final position %d, %d\n", node->final_position.row, node->final_position.col);
	printf("starting position %d, %d\n", node->starting_position.row, node->starting_position.col);
}

int letterToCol(char letter) {
	if ((int)letter <= 96 || (int)letter >= 105) {
		printf("letters should be between a-h. You inputed %c\n", letter);
	// 	exit(0);
	}
	return (int)letter - 97;
}

int ColToLetter(int col) {
	if ((int)col < 0 || (int)col > 7) {
		printf("letters should be between 0-7. You inputed %c\n", col);
		// exit(0);
	}
	return (int)col + 97;
}


struct dataTurn *toDataTurn(const char *original_move) {
    if (original_move[0] == 'x') {
        printf("moves cannot start with 'x'\n");
        return NULL; 
    }
    else {
        printf("first letter = %c\n", original_move[0]);
    }
    struct dataTurn *new_move = malloc(sizeof(struct dataTurn));
    char *list_of_pieces = "RNBQK";
    char *move_cpy = malloc(sizeof(original_move));
    char *move_cpy_save = move_cpy;
    strcpy(move_cpy, original_move);
     // deals with cheks and checkmates
    if (move_cpy[strlen(move_cpy) - 1] == '+' || move_cpy[strlen(move_cpy) - 1] == '#') {
        move_cpy[strlen(move_cpy) - 1] = '\0';
        new_move->is_check = true;
    }

    if (strchr(move_cpy, '=') != NULL) {
        new_move->piece_promotes_to = *(strchr(move_cpy, '=') + 1);
        
        move_cpy[strlen(move_cpy) - 2] = '\0';
    }
    else {
        new_move->piece_promotes_to = ' ';
    }
    
    if (strchr(move_cpy, 'O') != NULL) { // handles castling
        new_move->castles = true;
        if (strlen(move_cpy) <= 3) {
            new_move->is_king_side = true;
        }
        else {
            new_move->is_king_side = false;
        }
        return new_move;
    }
    else {
        new_move->castles = false;
    }

    new_move->piece = 'P';
    for (int i = 0; (size_t)i < strlen(list_of_pieces); i++) { // checks if it has a pawn or not
        if (strchr(move_cpy, list_of_pieces[i]) != NULL) {
            new_move->piece =  move_cpy[0];
            move_cpy = &move_cpy[1]; //cuts off first character if it is any of list_of_pieces
        }
    }

    struct standardPos final_pos = {.row = (move_cpy[strlen(move_cpy) - 1] - '0'), .col = letterToCol(move_cpy[strlen(move_cpy) - 2])}; // adds the final position of the piece
    new_move->final_position = final_pos;

    if (strchr(move_cpy, 'x') != NULL) { // specifies if the move is taking a piece
        new_move->takes =  true;
        move_cpy[strlen(move_cpy) - 1] = '\0';
    }
    else {
        new_move->takes =  false;
    }

    move_cpy[strlen(move_cpy) - 2] = '\0';
    struct standardPos restrictor_pos;
    if (strlen(move_cpy) == 1) {
       if (move_cpy[0] - '0' <= 7 && move_cpy[0] - '0' >= 0) {
           printf("new move cpy %c", move_cpy[0]);
			restrictor_pos.row = move_cpy[0] - '0' ;
			restrictor_pos.col = -1;
		}
		else {
			restrictor_pos.row = -1;
			restrictor_pos.col = letterToCol(move_cpy[0]);
		}
    }
    else if (strlen(move_cpy) == 2) {
        restrictor_pos.col = letterToCol(move_cpy[0]);
        restrictor_pos.row = move_cpy[1] - '0';
    }
    else {
        restrictor_pos.row = -1;
        restrictor_pos.col = -1;
    }

    free(move_cpy_save);
    new_move->restrictors = restrictor_pos;
    return new_move;
}

char *getMoveList(struct Move *head) {

    char *output = malloc(1000);
    char *buff = malloc(200);

    while(head != NULL) {
        sprintf(buff, "%d. %s %s ", head->move_number, head->white_notation, head->black_notation);
        strncat(output, buff, strlen(buff));
        head = head->next;
    }
    free(buff);


    return output;
}

void PrintMoveList(struct Move *head) {
    while(head != NULL) {
        printf("%d. %s %s ", head->move_number, head->white_notation, head->black_notation);
        head = head->next;
    }
}


struct standardPos posToStandard_pos(const struct Pos *input_pos) {
    struct standardPos result = {.row = input_pos->row, .col = input_pos->col};
    return result;
}

struct Pos standard_posToPos(const struct standardPos *input_pos) {
    struct Pos result = {.row = input_pos->row, .col = input_pos->col, .next = NULL};
    return result;
}

char oppositeSide(char side) {  // maybe this should overload some operator idk
	if (strchr("WB", side) != NULL) {
		return side == 'W' ? 'B' : 'W';
	}
	return side; //for no side
}

void printPosList(struct Pos *head) {
	while (head != NULL) {
		printf("row %d, col %d\n", head->row, head->col); // 
		head = head->next;
	}
}

void appendPos(struct Pos *head, int row, int col){
	while(head->next != NULL) {
		head = head->next;
	}
	struct Pos *newPos = malloc(sizeof(struct Pos));
	newPos->next = NULL;
	newPos->row = row;
	newPos->col = col;
	head->next = newPos;
}

void freePosList(struct Pos *head) {
    struct Pos *temphead;
    while(head != NULL) {
        
        temphead = head;
        head = head->next;
        free(temphead);
    }
}

bool posLlContains(struct Pos *head, struct Pos *to_compare) {
	while(head != NULL) {
		if (head->row == to_compare->row && head->col == to_compare->col) {
			return true;
		}	
		else {
		}
		head = head->next;
	}
	return false;
}
