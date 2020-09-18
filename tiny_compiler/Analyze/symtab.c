//
//  symtab.c
//  tiny_compiler
//
//  Created by linwenhu on 2020/9/18.
//  Copyright © 2020 wentilin. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"

/* Size of the hash table */
#define SIZE 211

/* Left shift */
#define SHIFT 4

static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        
        i++;
    }
    
    return temp;
}

typedef struct LineListRec {
    int lineno;
    struct LineListRec *next;
} *LineList;

typedef struct BucketListRec {
    char *name;
    LineList lines;
    int memloc;
    struct BucketListRec *next;
} *BucketList;

static BucketList hashTable[SIZE];

void st_insert(char *name, int lineno, int loc) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while (l != NULL && strcmp(name, l->name) != 0) {
        l = l->next;
    }
    
    if (l == NULL) {
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList)malloc(sizeof(struct LineListRec));
        l->lines->lineno = loc;
        l->memloc = loc;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    } else {
        LineList t = l->lines;
        while (t->next != NULL) {
            t = t->next;
        }
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
    }
}

int st_lookup(char *name) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while (l != NULL && strcmp(name, l->name) != 0) {
        l = l->next;
    }
    
    if (l == NULL) {
        return -1;
    } else {
        return l->memloc;
    }
}

void printSymTab(FILE *listing) {
    fprintf(listing, "Variable Name     Location    Line Numbers\n");
    fprintf(listing, "-------------     --------    ------------\n");
    for (int i = 0; i < SIZE; i++) {
        BucketList l = hashTable[i];
        while (l != NULL) {
            fprintf(listing, "%-14s     ", l->name);
            fprintf(listing, "%-8d     ", l->memloc);
            
            LineList t = l->lines;
            while (t != NULL) {
                fprintf(listing, "%4d ", t->lineno);
                t = t->next;
            }
                
            fprintf(listing, "\n");
            l = l->next;
        }
    }
}
