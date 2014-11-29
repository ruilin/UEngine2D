/*
 * hash.c
 *
 *  Created on: 2012-10-26
 *      Author: ottofung
 */
#include <string.h>

#include <util.h>

#include "hash.h"

struct _node {
	unsigned int key_len;
	unsigned int value_1;
	unsigned int value_2;
	struct _node *next;
	/* the key resides right after the structure */
	/* we don't put the key in the structure to prevent double allocation */
	/* we allocate in 1 go with sizeof(struct _node) to prevent memory fragmentation */
};

struct _bucket {
	/* each bucket points to the head of the linked list */
	struct _node *head;
};

struct _hash_stats {
	unsigned int currentAllocationBytes;
	unsigned int maxAllocationBytes;
	unsigned int bucketBytes;
	unsigned int collisionCount;
	unsigned int setCount;
	unsigned int getCount;
	unsigned int unsetCount;
	unsigned int replaceCount;
};

struct _hash {
	unsigned int bucket_count;
	unsigned int item_count;
	unsigned int empty_slot_count;
	struct _bucket *buckets;
	struct _hash_stats stats;
};

PUBLIC void *hash_init(unsigned char hashSizing) {
	/* 2^hashSizing = bucket count */
	/* 0 < hashSizing <= 32 */
	struct _hash *h;
	/* sanity check */
	if ((0 == hashSizing) || (32 < hashSizing)) {
		LOGE("hash_init(): hashSizing = %u", hashSizing);
		return NULL;
	}
	h = (struct _hash *)MALLOC(sizeof(struct _hash));
	if (NULL == h) {
		LOGE("hash_init(): MALLOC() failed.");
		return NULL;
	}
	h->stats.currentAllocationBytes = h->stats.maxAllocationBytes = h->stats.collisionCount = 0;
	h->stats.setCount = h->stats.getCount = h->stats.unsetCount = h->stats.replaceCount = 0;
	h->bucket_count = (1 << hashSizing); h->item_count = 0; h->empty_slot_count = h->bucket_count;
	h->buckets = (struct _bucket *)MALLOC(sizeof(struct _bucket) * h->bucket_count);
	memset((void *)h->buckets, 0, sizeof(struct _bucket) * h->bucket_count);	/* = sets all heads to NULL */
	h->stats.bucketBytes = sizeof(struct _bucket) * h->bucket_count;
	return (void *)h;
}

PUBLIC BOOL hash_set(void *hash, const void *key, unsigned int key_len, unsigned int value_1, unsigned int value_2) {
	struct _hash *h; unsigned int hashcode; struct _node *node; int fdone;
	h = (struct _hash *)hash;
	CRC32(0xFFFFFFFF, (const char *)key, key_len, hashcode);
	hashcode &= (h->bucket_count - 1);
	node = h->buckets[hashcode].head;
	fdone = 0;
	while (NULL != node) {
		if (key_len != node->key_len) {
			/* key_len not matched */
			node = node->next;
			continue;
		}
		if (0 == memcmp(key, node + 1, key_len)) {	/* node + 1 = access the memory after the node structure */
			/* found existing entry */ /* replace */
			node->value_1 = value_1; node->value_2 = value_2;
			(h->stats.replaceCount)++;
			fdone = 1;
			break;
		}
		node = node->next;
	}
	if (0 == fdone) {
		node = (struct _node *)MALLOC(sizeof(struct _node) + key_len);
		memcpy(node + 1, key, key_len);				/* copies the memory to the end of the node structure */
		node->key_len = key_len;
		node->value_1 = value_1; node->value_2 = value_2;
		/* it is a collision if head is not NULL */
		if (NULL != h->buckets[hashcode].head) {
			(h->stats.collisionCount)++;
		} else {
			(h->empty_slot_count)--;
		}
		node->next = h->buckets[hashcode].head;
		h->buckets[hashcode].head = node;
		(h->item_count)++;
		h->stats.currentAllocationBytes += sizeof(struct _node) + key_len;
		if (h->stats.maxAllocationBytes < h->stats.currentAllocationBytes) {
			h->stats.maxAllocationBytes = h->stats.currentAllocationBytes;
		}
	}
	(h->stats.setCount)++;
	return TRUE;
}

PUBLIC BOOL hash_get(void *hash, const void *key, unsigned int key_len, unsigned int *value_1, unsigned int *value_2) {
	struct _hash *h; unsigned int hashcode; struct _node *node;
	h = (struct _hash *)hash;
	CRC32(0xFFFFFFFF, (const char *)key, key_len, hashcode);
	hashcode &= (h->bucket_count - 1);
	node = h->buckets[hashcode].head;
	while (NULL != node) {
		if (key_len != node->key_len) {
			/* key_len not matched */
			node = node->next;
			continue;
		}
		if (0 == memcmp(key, node + 1, key_len)) {	/* node + 1 = access the memory after the node structure */
			if (NULL != value_1) *value_1 = node->value_1;
			if (NULL != value_2) *value_2 = node->value_2;
			(h->stats.getCount)++;
			return TRUE;
		}
		node = node->next;
	}
	return FALSE;
}

PUBLIC BOOL hash_unset(void *hash, const void *key, unsigned int key_len) {
	struct _hash *h; unsigned int hashcode; struct _node *node; struct _node *node_tmp;
	h = (struct _hash *)hash;
	CRC32(0xFFFFFFFF, (const char *)key, key_len, hashcode);
	hashcode &= (h->bucket_count - 1);
	node = h->buckets[hashcode].head;
	node_tmp = NULL;
	while (NULL != node) {
		if (key_len != node->key_len) {
			/* key_len not matched */
			node_tmp = node; node = node->next;
			continue;
		}
		if (0 == memcmp(key, node + 1, key_len)) {	/* node + 1 = access the memory after the node structure */
			/* found the entry */
			if (NULL == node_tmp) {	/* head node removal */
				h->buckets[hashcode].head = node->next;
				if (NULL == h->buckets[hashcode].head) {
					(h->empty_slot_count)++;
				}
			} else {
				node_tmp->next = node->next;
			}
			h->stats.currentAllocationBytes = h->stats.currentAllocationBytes - sizeof(struct _node) - key_len;
			node->key_len = 0; node->value_1 = node->value_2 = 0; node->next = NULL;
			FREE((void *)node);
			(h->item_count)--;
			(h->stats.unsetCount)++;
			return TRUE;
		}
		node_tmp = node; node = node->next;
	}
	return FALSE;
}

PUBLIC BOOL hash_enum(void *hash, HASH_ENUM_PROC proc, void *param) {
	/* enumerates and calls caller supplied proc until it returns FALSE or we finish all keys */
	/* NOTE: Caller MUST NOT "write/update" the hash during enumeration */
	struct _hash *h; unsigned int i; struct _node *node;
	h = (struct _hash *)hash;
	for (i = 0; i < h->bucket_count; i++) {
		/* for each bucket in the hash */
		if (NULL == h->buckets[i].head) continue;
		node = h->buckets[i].head;	/* gets the head node ... */
		do {						/* ... and traverse */
			if (FALSE == proc(node + 1, node->key_len, node->value_1, node->value_2, param))
				return FALSE;
			node = node->next;
		} while (NULL != node);
	}
	return TRUE;
}

PUBLIC void hash_clear(void *hash) {
	/* clears all values from the hash */
	struct _hash *h; unsigned int i;
	h = (struct _hash *)hash;
	/* frees up all the nodes in all buckets */
	for (i = 0; i < h->bucket_count; i++) {
		while (NULL != h->buckets[i].head) {
			struct _node *node; struct _node *node_tmp;
			node = h->buckets[i].head;
			node_tmp = node->next;
			node->key_len = 0;
			node->value_1 = node->value_2 = 0;
			FREE((void *)node);
			h->buckets[i].head = node_tmp;
		}
	}
	return;
}

PUBLIC void hash_destroy(void *hash) {
	struct _hash *h; unsigned int i;
	h = (struct _hash *)hash;
	/* frees up all the nodes in all buckets */
	for (i = 0; i < h->bucket_count; i++) {
		while (NULL != h->buckets[i].head) {
			struct _node *node; struct _node *node_tmp;
			node = h->buckets[i].head;
			node_tmp = node->next;
			node->key_len = 0;
			node->value_1 = node->value_2 = 0;
			FREE((void *)node);
			h->buckets[i].head = node_tmp;
		}
	}
	FREE((void *)h->buckets);
	FREE((void *)h);
	return;
}

PUBLIC void hash_stats_dump(void *hash, char *name ) {
	/* dump the stats to the log with the supplied name as the identifier */
	struct _hash *h;
	h = (struct _hash *)hash;
	LOGI("hash dump: %s", (NULL == name ? "" : name));
	LOGI("Set: %u Get: %u Unset: %u Replace: %u", h->stats.setCount, h->stats.getCount, h->stats.unsetCount, h->stats.replaceCount);
	LOGI("Item count: %u Empty slot: %u Collision: %u", h->item_count, h->empty_slot_count, h->stats.collisionCount);
	if (0 < h->item_count) {
		LOGI("Efficiency: %u%%", (100 * (h->bucket_count - h->empty_slot_count) / h->item_count));
	} else {
		LOGI("Efficiency: no item yet");
	}
	LOGI("Allocated bytes - max: %u current: %u buckets: %u", h->stats.maxAllocationBytes, h->stats.currentAllocationBytes, h->stats.bucketBytes);
	return;
}
