//
//  mem.c
//  tiny_compiler
//
//  Created by wentilin on 2020/9/20.
//  Copyright © 2020 wentilin. All rights reserved.
//

#include "mem.h"

#define m_NULL 0

static Header mem[MEMSIZE];
static Header *memptr = m_NULL;

void *m_malloc(unsigned nbytes) {
    Header *p, *newp;
    unsigned nunits;
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if (memptr == m_NULL) {
        memptr = mem;
        memptr->s.next = memptr;
        memptr->s.usedSize = 1;
        memptr->s.freeSize = MEMSIZE - 1;
    }
    
    for (p = memptr;
         (p->s.next != memptr && p->s.freeSize < nunits);
         p = p->s.next);
    
    if (p->s.freeSize < nunits) {
        return m_NULL;
    }
    
    newp = p + p->s.usedSize;
    newp->s.usedSize = nunits;
    newp->s.freeSize = p->s.freeSize - nunits;
    newp->s.next = p->s.next;
    p->s.freeSize = 0;
    p->s.next = newp;
    memptr = newp;
    
    return (void *)(newp + 1);
}


void m_free(void *ap) {
    Header *bp, *p, *prev;
    bp = (Header *)ap - 1;
    for (prev = memptr, p = memptr->s.next;
         (p != bp && p != memptr);
         prev = p, p = p->s.next);
    
    if (p != bp) {
        return;
    }
    
    prev->s.freeSize += p->s.usedSize + p->s.freeSize;
    prev->s.next = p->s.next;
    memptr = prev;
}
