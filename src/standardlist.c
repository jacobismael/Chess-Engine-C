#include "standardlist.h"

int lengthOfStandardList(const struct standardList *head) {
	int length = 0;	
	
	while(head != NULL) {
		length++;
		head = head->next;
	}

	return length;
}

void freeStandardList(struct standardList *head) {
	struct standardList *temp_head;

	while(head != NULL) {
		temp_head = head;
        head = head->next;
		free(temp_head->data);
        free(temp_head);
	}
}

void freeStandardListWithoutData(struct standardList *head) {
	struct standardList *temp_head;

	while(head != NULL) {
		temp_head = head;
        head = head->next;
        free(temp_head);
	}
}

struct standardList *getElementOfStandardList(struct standardList *head, int position) {	
    if (position > lengthOfStandardList(head)) {
        return NULL;
    }
    int length = 0;
	
	while(head != NULL) {
        if (length == position) {
            return head;
        }
		length++;
		head = head->next;
	}
    return NULL;
}

struct standardList *prependToStandardList(struct standardList *list_head,  void *data) {
    struct standardList *new_head = malloc(sizeof(struct standardList));
    new_head->data = data;
    new_head->next = list_head;
	return new_head;
}


struct standardList *appendToStandardList(struct standardList *list_head,  void *data) {
	struct standardList *head_cpy = list_head;
	while(list_head->next != NULL) {
		list_head = list_head->next;
	}
    struct standardList *new_node = malloc(sizeof(struct standardList));
    new_node->data = data;
    new_node->next = NULL;
	list_head->next = new_node;
	return head_cpy;
}

struct standardList *appendToStandardListReturnLast(struct standardList *list_head,  void *data) {
	while(list_head->next != NULL) {
		list_head = list_head->next;
	}
    struct standardList *new_node = malloc(sizeof(struct standardList));
    new_node->data = data;
    new_node->next = NULL;
	list_head->next = new_node;
	return new_node;
}