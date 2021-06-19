#include "move.h"

struct Result {
    
    float white_score;
    float black_score;

};

struct PGN {

    char event[256];
    char site[256];
    char date[256];
    int round;
    char white[256];
    char black[256];    

    
    struct Result* result;
    struct Move* move_head;

};

struct Result* MakeResult(float w_score, float b_score);
void PrintResult(struct Result* result);
void PrintGame(struct PGN* game);
void PrintGameStrict(struct PGN* game);
