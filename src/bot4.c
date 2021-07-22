//first decent bvot uses raw brute force
#include "bot4.h"

#define DEPTH 1

/*
	god I hate this

	o
	|
	|
	o---o------o
	|    \      \
	|     \      \
	o--o   \      \
            o--o   \
		            o--o--o
*/

void freeTree(struct boardNode *head) {
	struct boardNode* temp;
	while(head != NULL) {
		free(head->board);
		freeTree(head->child);
		free(head->turn);
		temp = head;
		head = head->next;
		free(temp);
	}
}

float bestMinScore(struct boardNode* head, bool aim_high /* this could be better named */) { // theres probably something wrong with this function
	if (head->child == NULL) {
		return head->score;
	}
	struct boardNode* best = head;
	float best_so_far = head->score;

	if (aim_high) {
		while(head != NULL) {
			if (head->score < best_so_far) {
				best_so_far = head->score;
				best = head;
			}
			head = head->next;
		}
	}
	else {
		while(head != NULL) {
			if (head->score > best_so_far) {
				best_so_far = head->score;
				best = head;
			}
			head = head->next;
		}
	}
	return bestMinScore(best->child, !aim_high);
}

struct fullDataTurn* bestNode(struct boardNode *head, const char side) {
	head = head->child; //this will be y own side
	struct fullDataTurn *best = head->turn;
	float current_best = -INFINITY;
	bool aim_high =  side == 'B' ? false : true;
		
	while(head != NULL) { 

		if (bestMinScore(head, aim_high) > current_best)	{
			current_best = bestMinScore(head, aim_high);
			best = head->turn;
		}
		head = head->next;
	}
	return best;

}

struct boardNode *generateTree(struct dataBoard* input_board, const char side, unsigned short depth) {
	(void)depth; //this is to ignore the warning about depth being unused
	struct boardNode *head = malloc(sizeof(struct boardNode));
	head->turn = NULL;
	head->board = input_board;
	head->score = getBasicBoardScore(input_board);
	head->next = NULL;
	head->child = NULL;

	buildOnTree(head, side, 1);

	return head;
}

void buildOnTree(struct boardNode *input_node, char side, unsigned short current_iteration) {
	if (current_iteration > DEPTH) {
		return;
	}
	addLayer(input_node, side);
	struct boardNode *temp = input_node->child;

	while (temp != NULL) {
		buildOnTree(temp, oppositeSide(side), current_iteration + 1);

		temp = temp->next;
	}
}


void addLayer(struct boardNode *input_node, const char side) {
	bool status = true;
	struct standardList *all_moves_list = allLegalMoves(input_node->board, side);
	struct standardList *all_moves_list_cpy = all_moves_list;
	struct dataBoard *original_board = input_node->board;
	struct dataBoard *edited_board = malloc(sizeof(struct dataBoard));

	while (all_moves_list != NULL) {
		//reset board to its original to simulate a new move
		memcpy(edited_board, original_board, sizeof(struct dataBoard));
		//makes new move	
		struct boardNode* new_node = malloc(sizeof(struct boardNode));
		new_node->turn = all_moves_list->data;
		new_node->child = NULL;
		new_node->board = buildFromHalfMove(edited_board, new_node->turn, side, &status);
		new_node->score = getBasicBoardScore(new_node->board);
		//prepend it to the next list
		new_node->next = input_node->child;
		input_node->child = new_node;

		//cycle through the moves
		all_moves_list = all_moves_list->next;	
	}
	//free the list we recieved but keep the data which should be freed with the rest of the tree
	freeStandardListWithoutData(all_moves_list_cpy);
}

struct fullDataTurn *bot4Choice(struct dataBoard *input_board, const char side, bool *status) {
	*status = true;
	struct boardNode* head = generateTree(input_board, side, 5);
	printf("test-------------------------------------------------------------\n");
	struct fullDataTurn* result = bestNode(head, side);
	return result;
}
