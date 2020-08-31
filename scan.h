/*
 * File: scan.h
 * The scanner interface for TINY COMPILER
 */

#ifndef _SCAN_H_
#define _SCAN_H_

/* Max size of token */
#define MAXTOKENLEN 40

/* Store the lexme of each token */
extern char tokenString[MAXTOKENLEN + 1];

TokenType getToken(void);

#endif