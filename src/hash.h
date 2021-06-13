#ifndef __HASH_H__
#define __HASH_H__

#include <stdbool.h>

typedef struct hashItem_t {
    uint64_t val;
} hashItem;

typedef struct hashTable_t {
    hashItem **table;
    char **words;
    uint64_t wordsIndex;
    uint64_t size;
    uint64_t size_multi;
} hashTable;

hashTable *createHashTable(uint64_t size);
void increaseHashTable(hashTable *hTable, uint32_t multiplier);
void freeHashTable(hashTable *hTable);
void freeHashItems(hashTable *hTable);
bool addHashItem(hashTable *hTable, char* key, uint64_t value);
unsigned long hashString(char* item);
uint64_t lookupVal(hashTable *hTable, char *item);
void addHashList(hashTable *hTable, char **items, uint64_t *vals);
uint64_t getOriginalSize(hashTable *hTable);

#endif
