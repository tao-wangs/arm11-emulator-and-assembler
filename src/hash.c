// simple hash table implementation, doesnt allow for removal of items

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "hash.h"

#define MAGIC_PRIME 7

hashTable *createHashTable(uint64_t size){
    
    hashTable *hTable = malloc(sizeof(hashTable));
    if (!(hTable)){
	printf("memory allocation failed for hashtable");
        exit(1); //malloc failed
    }

    hTable->size = size;
    hTable->size_multi = 1;
    hTable->table = malloc(hTable->size * sizeof(hashItem*));
    hTable->wordsIndex = 0;
    hTable->words = malloc(hTable->size * sizeof(char**));

    if(!(hTable->table)){
        printf("memory allocation failed for hashtable value table");
        exit(1);
    }

    if(!(hTable->words)){
        printf("memory allocation failed for hashtable word list");
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
    free(hTable->words);
    free(hTable->table);
    free(hTable);
}

void freeHashItems(hashTable *hTable){
    for(int i = 0; i < hTable->size; i++){
        free(hTable->table[i]);
    }
}

//please avoid this function and use addHashList where possible. This function does NOT handle collision conflicts
bool addHashItem(hashTable *hTable, char* key, uint64_t value){ 
    unsigned long hash = hashString(key);
    uint64_t index = hash % hTable->size;
    if(hTable->table[index]){
	//printf("index collision, at index %lu\n", index);
	return false;
    } else {
	hashItem *item = malloc(sizeof(hashItem));
	hTable->words[hTable->wordsIndex] = key;
        hTable->wordsIndex += 1; 
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

uint64_t lookupVal(hashTable *hTable, char *item){
    for(int i = 0; i < getOriginalSize(hTable); i++){
        if(!strcmp(item, hTable->words[i])){
            return (hTable->table[hashString(item) % hTable->size])->val;
	}
    }

    printf("%s\n", item);
    return 0; //item not added to hashList
}

void addHashList(hashTable *hTable, char **items, uint64_t *vals){
    for(int i = 0; i < getOriginalSize(hTable); i++){
        if(!(addHashItem(hTable, items[i], vals[i]))){ //will run in case of collision
  
	    hTable->size *= MAGIC_PRIME; //increase size
	    hTable->size_multi *= MAGIC_PRIME;

	    resetHashTable(hTable); //get a new table for values

	    addHashList(hTable, items, vals);//redistribute with increased size
	    break;
	}
    }
}

uint64_t getOriginalSize(hashTable *hTable){
    return hTable->size / hTable->size_multi;
}
