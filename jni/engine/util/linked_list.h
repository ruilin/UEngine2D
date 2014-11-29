/*
 * linked_list.h
 *	链表
 *  Created on: 2013-9-25
 *      Author: Ryan
 */

#ifndef linked_list_H_
#define linked_list_H_

#include <base.h>

typedef enum {
	LINKED_LIST_HEAD	= 0,
	LINKED_LIST_TAIL	= 1
} LINKED_LIST_ORIGIN;
typedef void (*CALLBACK_FUN_DESTROY)(void *data);

/*
 * data must extended Linked_item
 */
typedef struct Linked_Item {
	struct Linked_Item *prev;
	struct Linked_Item *next;
	void *data;
	BOOL isInList;
} Linked_Item;

/* call back when the item destroyed, can be seted NULL */
void *linked_list_create(CALLBACK_FUN_DESTROY data_destroy);
void linked_list_destroy(void *linkedList);
void linked_list_item_init(Linked_Item *item);
void linked_list_addItem(void *linked, Linked_Item *item, void *data);
void linked_list_addItem_frontOf(void *linked, Linked_Item *refItem, Linked_Item *item, void *data);
void linked_list_addItem_backOf(void *linked, Linked_Item *refItem, Linked_Item *item, void *data);
BOOL linked_list_isEmpty(void *linked);
BOOL linked_list_isExisted(void *linkedList, void *data);
void *linked_list_getHead(void *linked);
void *linked_list_getTail(void *linked);
unsigned short linked_list_getSize(void *linkedList);
void *linked_list_iterator(void *linked, LINKED_LIST_ORIGIN origin);
void *linked_list_iterator_next(void **iterator);
void *linked_list_iterator_prev(void **iterator);
void *linked_list_getNext(void *_item);
void *linked_list_getPrev(void *_item);
void linked_list_removeItem(void *linked, Linked_Item *item, BOOL needFreeData);
void linked_list_clean(void *linkedList);

#endif /* linked_list_H_ */
