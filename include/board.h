#include "move.h"
#include <stdbool.h>


struct piece {
	char pieceId;
	char side; //can be 'W' 'B' ' '
};

struct board {
    struct piece board[8][8]; 
	struct piece off_the_board[48];
};

struct board* setupBoard(void);
struct pos* findPos(struct board* inputBoard, struct piece* piece_to_find, char restrictor_row, char restrictor_col);
struct board* buildFromStart(struct board* inputBoard, struct Move* head);
char oppositeSide(char side);
char TeamOnSquare(struct board* inputBoard, int row, int col);
struct pos* appendPos(struct pos* head, int row, int col);
struct pos* listOfLegalMoves(struct board* input_board, struct pos* position, struct board* original_board);
struct board* buildFromMove(struct board* inputBoard, struct Move* move);
struct board* buildFromHalfMove(struct board* inputBoard, char* move, char side, int* status);
void printBoard(struct board* inputBoard);
void printPosList(struct pos* head);
bool posLlContains(struct pos* head, struct pos* to_compare);
