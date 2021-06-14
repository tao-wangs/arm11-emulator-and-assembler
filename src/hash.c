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

void freeHashItems(hashTable *hTable){
    for(int i = 0; i < hTable->size; i++){
        if(hTable->table[i]){
	    free(hTable->table[i]);
	}    
    }	
}

void freeHashTable(hashTable *hTable){
    freeHashItems(hTable);
    free(hTable->words);
    free(hTable->table);
    free(hTable);
}

bool addHashItem(hashTable *hTable, char* key, uint64_t value){ 
    unsigned long hash = hashString(key);
    uint64_t index = hash % hTable->size;
    hashItem *item = malloc(sizeof(hashItem));
    
    if(!(item)){
        printf("memory allocation failed for a hashItem");
    }

    if(hTable->table[index]){        
	if(!((hTable->table[index])->next)){
	    (hTable->table[index])->next = item;
	} else {
	    addLinkedItem(hTable->table[index], item); 
	}
    } else {
        hTable->table[index] = item;    
    }

    hTable->words[hTable->wordsIndex] = key;
    hTable->wordsIndex += 1; 
	
    item->key = key;
    item->val = value;
    item->next = NULL;
    
    return true;
}

void addLinkedItem(hashItem* item, hashItem* toLink){
    while(item->next){
        item = item->next;    
    }

    item->next = toLink;
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
            hashItem *hItem = hTable->table[hashString(item) % hTable->size];
	    
	    while(strcmp(item, hItem->key)){
	        hItem = hItem->next;
	    }

	    return hItem->val;
	}
    }
    return 0; //item not added to hashList
}

void addHashList(hashTable *hTable, char **items, uint64_t *vals){
    for(int i = 0; i < getOriginalSize(hTable); i++){
        addHashItem(hTable, items[i], vals[i]); //will run in case of collision
    }
}

uint64_t getOriginalSize(hashTable *hTable){
    //printf("size: %lu mult: %lu\n", hTable->size, hTable->size_multi);
    return hTable->size / hTable->size_multi;
}
