//
//  analyze.c
//  tiny_compiler
//
//  Created by linwenhu on 2020/9/18.
//  Copyright © 2020 wentilin. All rights reserved.
//

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* Counter for variable memory locations */
static int location = 0;

static void traverse(TreeNode *t,
                     void (* preProc)(TreeNode *t),
                     void (* postProc)(TreeNode *t)) {
    if (t != NULL) {
        preProc(t);
        
        for (int i = 0; i < MAXCHILDREN; i++) {
            traverse(t->child[i], preProc, postProc);
        }
        
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

static void nullProc(TreeNode *t) {
    return;
}

static void insertNode(TreeNode *t) {
    switch (t->nodeKind) {
        case StmtK:
            switch (t->kind.stmt) {
                case AssignK:
                case ReadK:
                    if (st_lookup(t->attr.name) == -1) {
                        st_insert(t->attr.name, t->lineno, location++);
                    } else {
                        st_insert(t->attr.name, t->lineno, 0);
                    }
                    break;
                default:
                    break;
            }
            break;
        case ExpK:
            switch (t->kind.exp) {
                case IdK:
                    if (st_lookup(t->attr.name) == -1) {
                        st_insert(t->attr.name, t->lineno, location++);
                    } else {
                        st_insert(t->attr.name, t->lineno, 0);
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

static void typeError(TreeNode *t, char *message) {
    fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
    Error = TRUE;
}

static void checkNode(TreeNode *t) {
    switch (t->nodeKind) {
        case ExpK:
            switch (t->kind.exp) {
                case OpK:
                    if (t->child[0]->type != Integer ||
                        t->child[1]->type != Integer) {
                        typeError(t, "Op applied to non-integer");
                    }
                    if (t->attr.op == EQ || t->attr.op == LT) {
                        t->type = Boolean;
                    } else {
                        t->type = Integer;
                    }
                    break;
                case ConstK:
                case IdK:
                    t->type = Integer;
                    break;
                default:
                    break;
            }
            break;
        case StmtK:
            switch (t->kind.stmt) {
                case IfK:
                    if (t->child[0]->type == Integer) {
                        typeError(t->child[0], "If test is not Boolean");
                    }
                    break;
                case AssignK:
                    if (t->child[0]->type != Integer) {
                        typeError(t->child[0], "Assignment of non-integer value");
                    }
                    break;
                case WriteK:
                    if (t->child[0]->type != Integer) {
                        typeError(t->child[0], "Write of non-integer value");
                    }
                    break;
                case RepeatK:
                if (t->child[1]->type == Integer) {
                    typeError(t->child[1], "Repeat test is not Boolean");
                }
                break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void buildSymtab(TreeNode *syntaxTree) {
    traverse(syntaxTree, insertNode, nullProc);
    
    if (TraceAnalyze) {
        fprintf(listing, "\nSymbol table: \n\n");
        printSymTab(listing);
    }
}

void typeCheck(TreeNode *syntaxTree) {
    traverse(syntaxTree, nullProc, checkNode);
}
