/*
 * File: util.h
 * Ultility functions for TINY COMPILER
 */

#ifndef _UTIL_H_
#define _UTIL_H_

void printToken(TokenType tokenType, const char *tokenString);

TreeNode *newStmtNode(StmtKind kind);
TreeNode *newExpNode(ExpKind kind);

char *copyString(char *s);

void printTree(TreeNode *tree);

#endif