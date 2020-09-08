#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

// hode the current token
static TokenType token;

/* function prototypes for recursive calls */
static TreeNode *stmt_sequence(void);
static TreeNode *statement(void);
static TreeNode *if_stmt(void);
static TreeNode *repeat_stmt(void);
static TreeNode *assign_stmt(void);
static TreeNode *read_stmt(void);
static TreeNode *write_stmt(void);
static TreeNode *exp(void);
static TreeNode *simple_exp(void);
static TreeNode *term(void);
static TreeNode *factor(void);

static void syntaxError(char *message) {
    fprintf(listing, "\n>> ");
    fprintf(listing, "Syntax error at line %d: %s", lineno, message);
    Error = TRUE;
}

static void match(TokenType expected) {
    if (token == expected) {
        token = getToken();
    } else {
        syntaxError("Unexpected token -> ");
        printToken(token, tokenString);
        fprintf(listing, "         ");
    }
}

/* stmt_sequence -> statement {; statement }*/
static TreeNode *stmt_sequence(void) {
    TreeNode *t = statement();
    TreeNode *p = t;
    while (token != ENDFILE && token != END && token != ELSE && token != UNTIL) {
        TreeNode *q;
        match(SEMI);
        q = statement();
        if (q != NULL) {
            if (t == NULL) {
                t = p = q;
            } else {
                p->sibling = q;
                p = q;
            }
        }
    }

    return t;
}

/* statment -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt */
static TreeNode *statement(void) {
    TreeNode *t = NULL;
    switch (token) {
        case IF: t = if_stmt(); break;
        case REPEAT: t = repeat_stmt(); break;
        case ID: t = assign_stmt(); break;
        case READ: t = read_stmt(); break;
        case WRITE: t = write_stmt(); break;
        default:
            syntaxError("Unexpected token -> ");
            printToken(token, tokenString);
            token = getToken();
            break;
    }

    return t;
}

/* if-stmt -> if exp then stmt-sequence [else stmt-sequence] end */
static TreeNode *if_stmt(void) {
    TreeNode *t = newStmtNode(IfK);
    match(IF);
    if (t != NULL) {
        t->child[0] = exp();
    }
    match(THEN);
    if (t != NULL) {
        t->child[1] = stmt_sequence();
    }
    if (token == ELSE) {
        match(ELSE);
        if (t != NULL) {
            t->child[2] = stmt_sequence();
        }
    }
    match(END);

    return t;
}

/* repeat-stmt -> repeat stmt-sequence until exp */
static TreeNode *repeat_stmt(void) {
    TreeNode *t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != NULL) {
        t->child[0] = stmt_sequence();
    }
    match(UNTIL);
    if (t != NULL) {
        t->child[1] = exp();
    }

    return t;
}

/* assign-stmt -> identifier := exp */
static TreeNode *assign_stmt(void) {
    TreeNode *t = newStmtNode(AssignK);
    if (t != NULL && token == ID) {
        t->attr.name = copyString(tokenString);
    }

    match(ID);
    match(ASSIGN);
    
    if (t != NULL) {
        t->child[0] = exp();
    }

    return t;
}

/* read-stmt -> read identifier */
static TreeNode *read_stmt(void) {
    TreeNode *t = newStmtNode(ReadK);
    match(READ);

    if (t != NULL && token == ID) {
        t->attr.name = copyString(tokenString);
    }

    match(ID);

    return t;
}

/* write-stmt -> write exp */
static TreeNode *write_stmt(void) {
TreeNode *t = newStmtNode(WriteK);
    match(WRITE);

    if (t != NULL && token == ID) {
        t->child[0] = exp();
    }

    match(ID);

    return t;
}

/* exp -> simple-exp [comparison-op simple-exp] */
static TreeNode *exp(void) {
    TreeNode *t = simple_exp();
    if (token == LT || token == EQ) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
        }

        match(token);
        if (t != NULL) {
            t->child[1] = simple_exp();
        }
    }

    return t;
}

/* simple-exp -> term { addop term } */
static TreeNode *simple_exp(void) {
    TreeNode *t = term();
    while (token == PLUS || token == MINUS) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            t->attr.op = token;
            t = p;
            match(token);
            t->child[1] = term();
        }
    }

    return t;
}

/* term -> factor { mulop factor } */
static TreeNode *term(void) {
    TreeNode *t = factor();
    while (token == TIMES || token == OVER) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            t->attr.op = token;
            t = p;
            match(token);
            t->child[1] = factor();
        }
    }

    return t;
}

/* factor -> (exp) | number | identifier */
static TreeNode *factor(void) {
    TreeNode *t = NULL;
    switch (token) {
        case NUM:
            t = newExpNode(ConstK);
            if (t != NULL) {
                t->attr.val = atoi(tokenString);
            }

            match(NUM);
            break;
        case ID:
            t = newExpNode(IdK);
            if (t != NULL) {
                t->attr.name = copyString(tokenString);
            }

            match(ID);
            break;
        case LPAREN:
            match(LPAREN);
            t = exp();
            match(RPAREN);
            break;
        default:
            syntaxError("Unexpect token -> ");
            printToken(token, tokenString);
            token = getToken();
            break;
    }

    return t;
}

TreeNode *parse(void) {
    TreeNode *t;
    token = getToken();
    t = stmt_sequence();
    if (token != ENDFILE) {
        syntaxError("Code ends before file\n");
    }

    return t;
}
