//
//  symtab.h
//  tiny_compiler
//
//  Created by linwenhu on 2020/9/18.
//  Copyright © 2020 wentilin. All rights reserved.
//

#ifndef symtab_h
#define symtab_h

/* Insert symbol */
void st_insert(char *name, int lineno, int loc);

/*Lookup symbol by name */
int st_lookup(char *name);

/* Print symbol table formated */
void printSymTab(FILE *listing);

#endif /* symtab_h */
