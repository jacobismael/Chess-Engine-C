//first decent bvot uses raw brute force
#include "bot4.h"


//this needs to be odd I thyink
#define DEPTH 8


void prependToChildren(struct branch *input_branch, struct boardMove new_data) {
	struct branch *new_child = malloc(sizeof(struct branch));
	new_child->next = input_branch->child;
	new_child->child = NULL;
	new_child->data = new_data;
	new_child->parent = input_branch;
	input_branch->child = new_child;
}

struct branch *generateTree(const struct dataBoard *input_board, const char side) {
	bool status = true;
	struct branch *head = malloc(sizeof(struct branch));
	head->next = NULL;
	head->parent = NULL;
	head->child = NULL;

	struct standardList *all_moves = allLegalMoves(input_board, side);
	struct standardList *all_moves_cpy = all_moves;
	struct dataBoard *editable_board = malloc(sizeof(struct dataBoard));

	while (all_moves != NULL) {
		memcpy(editable_board, input_board, sizeof(struct dataBoard));
		struct boardMove new_boardMove;
		new_boardMove.turn = *(struct fullDataTurn *)all_moves->data;
		new_boardMove.board = *buildFromHalfMove(editable_board, all_moves->data, side, &status);
		new_boardMove.score = getBoardScore(&new_boardMove.board);

		prependToChildren(head, new_boardMove);
		addToTree(head->child, &new_boardMove.board, oppositeSide(side), 0);
		all_moves = all_moves->next;
	}
	freeStandardListWithoutData(all_moves_cpy);
	return head;
}

void addToTree(struct branch *head, const struct dataBoard *input_board, const char side, unsigned char depth_reached) {
	if (depth_reached >= DEPTH) {
		return;
	}
	bool status = true;
	struct standardList *all_moves = allLegalMoves(input_board, side);
	if (all_moves == NULL) {
		return;
	}
	struct standardList *all_moves_cpy = all_moves;
	struct dataBoard *editable_board = malloc(sizeof(struct dataBoard));

	while (all_moves != NULL) {
		memcpy(editable_board, input_board, sizeof(struct dataBoard));
		struct boardMove new_boardMove;
		new_boardMove.turn = *(struct fullDataTurn *)all_moves->data;
		new_boardMove.board = *buildFromHalfMove(editable_board, all_moves->data, side, &status);
		new_boardMove.score = getBoardScore(&new_boardMove.board);
		if (new_boardMove.score < 0) { // everything inside this if statement including the statement itself only works for white
			bool should_end = true;
			struct branch *temp = head;
			for (int i = 0; i < 3; i++) {
				if (temp->parent == NULL) {
					break;
				}
				else if (temp->parent->data.score >= 0){
					should_end = false;
					break;	
				}
				temp = temp->parent;
			}
			if (should_end) {
				return;
			}
		}
		prependToChildren(head, new_boardMove);
		if (new_boardMove.score == INFINITY  || new_boardMove.score == -INFINITY) {
			return;
		}
		addToTree(head->child, &new_boardMove.board, oppositeSide(side), ++depth_reached);
		all_moves = all_moves->next;
	}
	freeStandardListWithoutData(all_moves_cpy);
	return;
	
}

struct fullDataTurn *getBest(const struct branch *head, const char starting_side) {
	struct standardList *best_moves = NULL;
	float best_score = starting_side == 'W' ? -INFINITY : INFINITY;
	float temp;
	head = head->child;
	while (head !=  NULL) {
		temp = getBestScoreOfBranch(head, starting_side == 'W');
		//printf("%f\n", *temp);
		if (temp > best_score) {
			best_score = temp;
			printf("new high: %f\n", best_score);
			freeStandardList(best_moves);
			best_moves = NULL;
			struct fullDataTurn *move = malloc(sizeof(struct fullDataTurn));
			memcpy(move, &(head->data.turn), sizeof(struct fullDataTurn));
			best_moves = prependToStandardList(best_moves, move);
			//*best_so_far = head->data.turn;
		}
		else if (temp == best_score) {
			struct fullDataTurn *move = malloc(sizeof(struct fullDataTurn));
			memcpy(move, &(head->data.turn), sizeof(struct fullDataTurn));
			best_moves = prependToStandardList(best_moves, move);
		}

		head = head->next;
	}
	printf("chosen %f\n", best_score);
	int len = lengthOfStandardList(best_moves);
	int test = randomInt(0, len);
	return (getElementOfStandardList(best_moves, test))->data;
}

float getBestScoreOfBranch(const struct branch *head, bool aim_high) {

	if (head->child == NULL) {
		//printf("test %f\n", head->data.score);
		return head->data.score;
	}
	else {
		head = head->child;
		float best = getBestScoreOfBranch(head, !aim_high);
		head = head->next;
		while (head != NULL) {
			if ((getBestScoreOfBranch(head, !aim_high) > best && aim_high) || (getBestScoreOfBranch(head, !aim_high) < best && !aim_high)) {
				best = head->data.score;
			}
			head = head->next;
		}

		return best;
	}
}


struct fullDataTurn *bot4Choice(const struct dataBoard *input_board, const char side, bool *status) {
	struct branch *temp = generateTree(input_board, side);
	*status = true;
	return getBest(temp, side);
}
