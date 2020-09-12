/*
 * File: util.h
 * Ultility function implementation for TINY COMPILER
 */

#include "globals.h"
#include "util.h"

void printToken(TokenType tokenType, const char *tokenString) {
    switch (tokenType) {
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
            fprintf(listing, "reserved word: %s\n", tokenString);
            break;
        case ASSIGN: fprintf(listing, ":=\n"); break;
        case LT: fprintf(listing, "<\n"); break;
        case EQ: fprintf(listing, "=\n"); break;
        case LPAREN: fprintf(listing, "(\n"); break;
        case RPAREN: fprintf(listing, ")\n"); break;
        case SEMI: fprintf(listing, ";\n"); break;
        case PLUS: fprintf(listing, "+\n"); break;
        case MINUS: fprintf(listing, "-\n"); break;
        case TIMES: fprintf(listing, "*\n"); break;
        case OVER: fprintf(listing, "/\n"); break;
        case ENDFILE: fprintf(listing, "EOF\n"); break;
        case NUM: fprintf(listing, "NUM, val=%s\n", tokenString); break;
        case ID: fprintf(listing, "ID, name=%s\n", tokenString); break;
        case ERROR: fprintf(listing, "ERROR: %s\n", tokenString); break;
        default:
            fprintf(listing, "Unknown token: %s\n", tokenString); break;
            break;
    }
}

TreeNode *newStmtNode(StmtKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    } else {
        for (int i = 0; i < MAXCHILDREN; i++) {
            t->child[i] = NULL;
        }

        t->sibling = NULL;
        t->nodeKind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }

    return t;
}

TreeNode *newExpNode(ExpKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    } else {
        for (int i = 0; i < MAXCHILDREN; i++) {
            t->child[i] = NULL;
        }

        t->sibling = NULL;
        t->nodeKind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }

    return t;
}

char *copyString(char *s) {
    size_t n;
    char *t;
    if (s == NULL) {
        return NULL;
    }

    n = strlen(s);
    t = malloc(n);
    if (t == NULL) {
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    } else {
        strcpy(t, s);
    }

    return t;
}

static int indentno = 0;

#define INDENT indentno += 2
#define UNINDENT indentno -= 2

static void printSpaces() {
    for (int i = 0; i < indentno; i++) {
        fprintf(listing, "  ");
    }
}

void printTree(TreeNode *tree) {
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodeKind == StmtK) {
            switch (tree->kind.stmt) {
                case IfK:
                    fprintf(listing, "If\n");
                    break;
                case RepeatK:
                    fprintf(listing, "Repeat\n");
                    break;
                case AssignK:
                    fprintf(listing, "Assign to: %s\n", tree->attr.name);
                    break;
                case ReadK:
                    fprintf(listing, "Read: %s\n", tree->attr.name);
                    break;
                case WriteK:
                    fprintf(listing, "Write\n");
                    break;
                default:
                    fprintf(listing, "Unknown ExpNode kind\n");
            }
        } else if (tree->nodeKind == ExpK) {
            switch (tree->kind.exp) {
                case OpK:
                    fprintf(listing, "Op: ");
                    printToken(tree->attr.op, "\0");
                    break;
                case ConstK:
                    fprintf(listing, "Const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    fprintf(listing, "Id: %s\n", tree->attr.name);
                    break;
                default:
                    fprintf(listing, "Unknown ExpNode kind\n");
            }
        } else {
            fprintf(listing, "Unknown node kind\n");
        }

        for (int i = 0; i < MAXCHILDREN; i++) {
            printTree(tree->child[i]);
        }

        tree = tree->sibling;
    }

    UNINDENT;
}
