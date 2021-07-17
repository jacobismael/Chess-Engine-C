#ifndef STANDARDLIST_H
#define STANDARDLIST_H
#include <stdlib.h>

struct standardList {
    struct standardList* next;
    void* data;
};

struct standardList* prependToStandardList(struct standardList* list_head,  void const* data);
void freeStandardList(struct standardList *head);
void freeStandardListWithoutData(struct standardList *head);
int lengthOfStandardList(const struct standardList *head);
struct standardList *getElementOfStandardList(const struct standardList *head, int position);


#endif