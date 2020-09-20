//
//  mem.h
//  tiny_compiler
//
//  Created by wentilin on 2020/9/20.
//  Copyright © 2020 wentilin. All rights reserved.
//

#ifndef mem_h
#define mem_h

#include <stdio.h>

#define MEMSIZE 8096

typedef double Align;
typedef union header {
    struct {
        union header *next;
        unsigned usedSize;
        unsigned freeSize;
    } s;
    Align a;
} Header;

void *m_malloc(unsigned nbytes);

void m_free(void *np);

#endif /* mem_h */
