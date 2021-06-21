#include "move.h"

struct pos { 
	int row;
	int col;
	struct pos*  next; //optional
};

struct piece {
	char pieceId;
	char side; //can be 'W' 'B' ' '
};

struct board {
    struct piece board[8][8]; 
	struct piece off_the_board[48];
};

struct board* setupBoard(void);
int letterToCol(char letter);
struct pos* findPos(struct board* inputBoard, struct piece* piece_to_find, char restrictor_row, char restrictor_col);
struct board* buildFromStart(struct board* inputBoard, struct Move* head);
char oppositeSide(char side);
char TeamOnSquare(struct board* inputBoard, int row, int col);
struct pos* appendPos(struct pos* head, int row, int col);
struct pos* listOfLegalMoves(struct board* inputBoard, struct pos* position);
struct board* buildFromMove(struct board* inputBoard, struct Move* move);
void printBoard(struct board* inputBoard);
void printPosList(struct pos* head);
bool posLlContains(struct pos* head, struct pos* to_compare);
