#ifndef __HASH_H__
#define __HASH_H__

#include <stdbool.h>
typedef struct hashItem_t hashItem;

struct hashItem_t {
    char* key;
    uint64_t val;
    hashItem* next;
};

typedef struct hashTable_t {
    hashItem **table;
    char **words;
    uint64_t wordsIndex;
    uint64_t size;
    uint64_t size_multi;
} hashTable;

hashTable *createHashTable(uint64_t size);
void freeHashTable(hashTable *hTable);
void freeHashItems(hashTable *hTable);
bool addHashItem(hashTable *hTable, char* key, uint64_t value);
void addLinkedItem(hashItem *item, hashItem *toLink);
unsigned long hashString(char* item);
uint64_t lookupVal(hashTable *hTable, char *item);
void addHashList(hashTable *hTable, char **items, uint64_t *vals);
uint64_t getOriginalSize(hashTable *hTable);

#endif
