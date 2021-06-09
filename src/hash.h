#ifndef __HASH_H__
#define __HASH_H__

typedef struct hashItem_t {
    uint32_t val;
} hashItem;

typedef struct hashTable_t {
    hashItem **table;
    uint32_t size;
} hashTable;

hashTable *createHashTable(uint32_t size);
void freeHashTable(hashTable *hTable);
bool addHashItem(hashTable *hTable, char* key, uint32_t value);
unsigned long hashString(char* item);
uint32_t lookupVal(hashTable *hTable, char *item);
bool addHashList(hashTable *hTable, char **items, uint32_t *vals);

#endif
