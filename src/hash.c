// simple hash table implementation, doesnt allow for removal of items

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "hash.h"

hashTable *createHashTable(uint32_t size){
    uint32_t tableSize = 2 * size; // avoid conflicts
    hashTable *hTable = malloc(sizeof(hashTable));
    if (!(hTable)){
	printf("memory allocation failed for hashtable");
        exit(1); //malloc failed
    }

    hTable->size = tableSize;
    hTable->table = malloc(tableSize * sizeof(hashItem*));
    if(!(hTable->table)){
        printf("memory allocation failed for hashtable value table");
	exit(1);
    }
    for(int i = 0; i < tableSize; i++){
        hTable->table[i] = NULL;
    }

    return hTable;
}

void freeHashTable(hashTable *hTable){
    free(hTable->table);
    free(hTable);
}

bool addHashItem(hashTable *hTable, char* key, uint32_t value){
    unsigned long hash = hashString(key);
    uint32_t index = hash % hTable->size;
    if(hTable->table[index]){
        printf("index collision, aborting creation...");
	return false;
    } else {
	hashItem *item = malloc(sizeof(hashItem));
        
	if(!(item)){
	    printf("memory allocation failed for a hashItem");
	}

	item->val = value;
        hTable->table[index] = item;
	return true;
    }
}

unsigned long hashString(char *item){ //using djb2 hashing algo
    unsigned long hash = 5381; 
    int c;
    while((c = *item++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

uint32_t lookupVal(hashTable *hTable, char *item){
     return (hTable->table[hashString(item) % hTable->size])->val;
}

bool addHashList(hashTable *hTable, char **items, uint32_t *vals){
    for(int i = 0; i < ((hTable->size) / 2); i++){
        if(!(addHashItem(hTable, items[i], vals[i]))){
	    return false; //collision
	}
    }

    return true; //all items added, no collisions
}

