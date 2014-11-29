/*
 * linked_list.c
 *
 *  Created on: 2013-9-25
 *      Author: Ryan
 */

#include "linked_list.h"


typedef struct Linked_list {
	CALLBACK_FUN_DESTROY data_destroy;
	Linked_Item *itemHead;
	Linked_Item *itemTail;
	unsigned short size;
} Linked_list;

PUBLIC void *linked_list_create(CALLBACK_FUN_DESTROY data_destroy) {
	Linked_list *linkedList;
	linkedList = MALLOC(sizeof(struct Linked_list));
	if (NULL == linkedList) {
		LOGE("linked_list_create(): MALLOC fail, NULL==linkedList");	return NULL;
	}
	linkedList->data_destroy = data_destroy;
	linkedList->size = 0;
	linkedList->itemHead = linkedList->itemTail = NULL;
	return (void *)linkedList;
}

PUBLIC void linked_list_destroy(void *linkedList) {
	if (NULL == linkedList) return;
	linked_list_clean(linkedList);
	FREE(linkedList);
	return;
}

/* 链表节点必须调用该函数进行初始化 */
PUBLIC void linked_list_item_init(Linked_Item *item) {
	item->data = NULL;
	item->prev = item->next = NULL;
	item->isInList = FALSE;
	return;
}

PRIVATE Linked_Item *_createItem() {
	Linked_Item *linkedListItem = MALLOC(sizeof(Linked_Item));
	if (NULL == linkedListItem) {
		LOGE("linked_list_createItem(): MALLOC fail, NULL == linkedList");
		return NULL;
	}
	linkedListItem->data = NULL;
	linkedListItem->prev = linkedListItem->next = NULL;
	return linkedListItem;
}

PRIVATE void _destroyItem(Linked_Item *item) {
	if (NULL == item) return;
	FREE(item);
	return;
}

//PUBLIC void linked_list_insertItem(Linked_item **itemHead, Linked_item **itemTail, Linked_item *item) {
//	item->next = *itemHead;
//	if (NULL != *itemHead) {
//		(*itemHead)->prev = item;
//	}
//	*itemHead = item;
//	if (NULL == *itemTail) {
//		*itemTail = *itemHead;
//	}
//	return;
//}

//PUBLIC void linked_list_addItem(void *linked, void *data) {
//	Linked_list *linkedList = (Linked_list *)linked;
//	Linked_item *item = _createItem();
//	item->data = data;
//
//	item->next = linkedList->itemHead;
//	if (NULL != linkedList->itemHead) {
//		linkedList->itemHead->prev = item;
//	}
//	linkedList->itemHead = item;
//	if (NULL == linkedList->itemTail) {
//		linkedList->itemTail = linkedList->itemHead;
//	}
//	linkedList->size++;
//	return;
//}

/* 添加节点 */
PUBLIC void linked_list_addItem(void *linked, Linked_Item *item, void *data) {
	Linked_list *linkedList = (Linked_list *)linked;
	item->data = data;
	item->prev = NULL;
	item->next = linkedList->itemHead;
	if (NULL != linkedList->itemHead) {
		linkedList->itemHead->prev = item;
	}
	linkedList->itemHead = item;
	if (NULL == linkedList->itemTail) {
		linkedList->itemTail = linkedList->itemHead;
	}
	item->isInList = TRUE;
	linkedList->size++;
	return;
}

/* add item in front of refItem*/
PUBLIC void linked_list_addItem_frontOf(void *linked, Linked_Item *refItem, Linked_Item *item, void *data) {
	Linked_list *linkedList = (Linked_list *)linked;
	item->data = data;
	if (NULL != refItem->prev) {
		refItem->prev->next = item;
	} else {
		linkedList->itemHead = item;
	}
	item->prev = refItem->prev;
	item->next = refItem;
	refItem->prev = item;

	item->isInList = TRUE;
	linkedList->size++;
	return;
}

/* add item at the back of refItem*/
PUBLIC void linked_list_addItem_backOf(void *linked, Linked_Item *refItem, Linked_Item *item, void *data) {
	Linked_list *linkedList = (Linked_list *)linked;
	item->data = data;
	if (NULL != refItem->next) {
		refItem->next->prev = item;
	} else {
		linkedList->itemTail = item;
	}
	item->next = refItem->next;
	item->prev = refItem;
	refItem->next = item;

	item->isInList = TRUE;
	linkedList->size++;
	return;
}

PUBLIC BOOL linked_list_isEmpty(void *linked) {
	Linked_list *linkedList = (Linked_list *)linked;
	return (NULL == linkedList->itemHead);
}

PUBLIC BOOL linked_list_isExisted(void *linked, void *data) {
	Linked_list *linkedList = (Linked_list *)linked;
	Linked_Item *item = NULL;
	Linked_Item *tempitem = linkedList->itemHead;
	while (NULL != tempitem) {
		if (tempitem->data == data) {
			item = tempitem;
			break;
		}
		tempitem = tempitem->next;
	}
	return (NULL != item);
}

PRIVATE void *linked_list_getItem(void *linked, const unsigned short index) {
	Linked_list *linkedList = (Linked_list *)linked;
	unsigned short i = 0;
	Linked_Item *item = linkedList->itemHead;
	while (NULL != item){
		if (i == index) {
			break;
		}
		item = item->next;
		i++;
	}
	return (NULL == item) ? NULL : item->data;
}

PUBLIC void *linked_list_getHead(void *linked) {
	if (NULL == linked) {
		LOGE("linked_list_getHead() error  linked == NULL");
	}
	Linked_list *linkeList = (Linked_list *)linked;
	return (NULL == linkeList->itemHead ? NULL : linkeList->itemHead->data);
}

PUBLIC void *linked_list_getTail(void *linked) {
	if (NULL == linked) {
		LOGE("linked_list_getTail() error  linked == NULL");
	}
	Linked_list *linkeList = (Linked_list *)linked;
	return (NULL == linkeList->itemTail ? NULL : linkeList->itemTail->data);
}

PUBLIC void *linked_list_iterator(void *linked, LINKED_LIST_ORIGIN origin) {
	switch(origin) {
	case LINKED_LIST_HEAD:
		return (void *)(((Linked_list *)linked)->itemHead);
	case LINKED_LIST_TAIL:
		return (void *)(((Linked_list *)linked)->itemTail);
	}
}

PUBLIC void *linked_list_iterator_next(void **iterator) {
	Linked_Item *item = (Linked_Item *)*iterator;
	*iterator = item->next;
	return item->data;
}

PUBLIC void *linked_list_iterator_prev(void **iterator) {
	Linked_Item *item = (Linked_Item *)*iterator;
	*iterator = item->prev;
	return item->data;
}

PUBLIC void *linked_list_getNext(void *_item) {
	Linked_Item *item = (Linked_Item *)_item;
	if (NULL == item || NULL ==item->next) return NULL;
	return item->next->data;
}

PUBLIC void *linked_list_getPrev(void *_item) {
	Linked_Item *item = (Linked_Item *)_item;
	return item->prev->data;
}

PUBLIC unsigned short linked_list_getSize(void *linked) {
	return ((Linked_list *)linked)->size;
}

//PUBLIC void linked_list_removeItem(Linked_item **itemHead, Linked_item **itemTail, Linked_item *item) {
//	if (NULL == item) return;
//	if (NULL != item->prev) {
//		item->prev->next = item->next;
//	} else {
//		*itemHead = item->next;
//	}
//	if (NULL != item->next) {
//		item->next->prev = item->prev;
//	} else {
//		*itemTail = item->prev;
//	}
//	item->prev = item->next = NULL;
//	return;
//}

/*
 * 删除节点
 */
PUBLIC void linked_list_removeItem(void *linked, Linked_Item *item, BOOL needFreeData) {
	if (NULL == item || FALSE == item->isInList) return;
	Linked_list *linkedList = (Linked_list *)linked;
	if (NULL != item->prev) {
		item->prev->next = item->next;
	} else {
		linkedList->itemHead = item->next;
	}
	if (NULL != item->next) {
		item->next->prev = item->prev;
	} else {
		linkedList->itemTail = item->prev;
	}
	item->prev = item->next = NULL;
	if (TRUE == needFreeData && NULL != linkedList->data_destroy) {
		linkedList->data_destroy(item->data);
		item->data = NULL;
	}
//	_destroyItem(item);
	item->isInList = FALSE;
	linkedList->size--;
	return;
}

PRIVATE void linked_list_removeItemByIndex(void *linked, int index) {
	Linked_list *linkedList = (Linked_list *)linked;
	Linked_Item *item = NULL;
	Linked_Item *tempitem = linkedList->itemHead;
	int i = 0;
	while (NULL != tempitem) {
		if (i == index) {
			item = tempitem;
			break;
		}
		tempitem = tempitem->next;
	}
	if (NULL == item) return;
	if (NULL != item->prev) {
		item->prev->next = item->next;
	} else {
		linkedList->itemHead = item->next;
	}
	if (NULL != item->next) {
		item->next->prev = item->prev;
	} else {
		linkedList->itemTail = item->prev;
	}
	item->prev = item->next = NULL;
	if (NULL != linkedList->data_destroy)
		linkedList->data_destroy(item->data);
//	_destroyItem(item);
	item->isInList = FALSE;
	linkedList->size--;
	return;
}

//PUBLIC void linked_list_clean(Linked_item **itemHead, Linked_item **itemTail) {
//	Linked_item *item = *itemHead;
//	while (NULL != item) {
//		Linked_item *nextItem = item->next;
//		linked_list_destroyItem(item);
//		item = nextItem;
//	}
//	*itemHead = *itemTail = NULL;
//	return;
//}

/*
 * 清空链表
 */
PUBLIC void linked_list_clean(void *linked) {
	Linked_list *linkedList = (Linked_list *)linked;
	Linked_Item *item = linkedList->itemHead;
	while (NULL != item) {
		Linked_Item *nextItem = item->next;
		item->isInList = FALSE;
		if (NULL != linkedList->data_destroy)
			linkedList->data_destroy(item->data);
//		_destroyItem(item);
		linkedList->size--;
		item = nextItem;
	}
	linkedList->itemHead = linkedList->itemTail = NULL;
	return;
}
