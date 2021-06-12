// simple hash table implementation, doesnt allow for removal of items

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "hash.h"

#define MAGIC_PRIME 7

hashTable *createHashTable(uint32_t size){
    
    hashTable *hTable = malloc(sizeof(hashTable));
    if (!(hTable)){
	printf("memory allocation failed for hashtable");
        exit(1); //malloc failed
    }

    hTable->size = size * MAGIC_PRIME;
    hTable->size_multi = MAGIC_PRIME;
    hTable->table = malloc(hTable->size * sizeof(hashItem*));

    if(!(hTable->table)){
        printf("memory allocation failed for hashtable value table");
        exit(1);
    }

    for(int i = 0; i < hTable->size ; i++){
        hTable->table[i] = NULL;
    }

    return hTable;
}

void resetHashTable(hashTable *hTable){
    free(hTable->table);

    hTable->table = malloc(hTable->size * sizeof(hashItem*));

    if(!(hTable->table)){
        printf("memory allocation failed for hashtable value table");
        exit(1);
    }

    for(int i = 0; i < hTable->size; i++){
        hTable->table[i] = NULL;
    }
}

void freeHashTable(hashTable *hTable){
    freeHashItems(hTable);
    free(hTable->table);
    free(hTable);
}

void freeHashItems(hashTable *hTable){
    for(int i = 0; i < hTable->size; i++){
        free(hTable->table[i]);
    }
}

bool addHashItem(hashTable *hTable, char* key, uint32_t value){
    unsigned long hash = hashString(key);
    uint32_t index = hash % hTable->size;
    if(hTable->table[index]){
	printf("index collision, at index %u\n", index);
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

unsigned long hashString(char *item){ 
    unsigned long hash = 5381; 
    int c;
    while((c = *item++)){
        hash = (hash << 5) + hash + c;
    }
    return hash;
}

uint32_t lookupVal(hashTable *hTable, char *item){
     return (hTable->table[hashString(item) % hTable->size])->val;
}

bool addHashList(hashTable *hTable, char **items, uint32_t *vals){
    for(int i = 0; i < ((hTable->size) / (hTable->size_multi)); i++){
        if(!(addHashItem(hTable, items[i], vals[i]))){ //will run in case of collision
	    freeHashItems(hTable); //delete all hash values so far
	    
	    hTable->size *= MAGIC_PRIME; //increase size
	    hTable->size_multi *= MAGIC_PRIME;

	    resetHashTable(hTable); //get a new table for values

	    addHashList(hTable, items, vals);//redistribute with increased size
	}
    }

    return true; //all items added, no collisions
}
