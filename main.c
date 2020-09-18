#include "globals.h"
#include "util.h"
#include "analyze.h"

#define NO_PARSE FALSE

#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#endif

int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;
int Error = FALSE;

int main(int argc, char const *argv[]) {
    // source code file
    TreeNode *syntaxTree;
    char pgm[100];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL) {
        strcat(pgm, ".tny");
    }

    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "File %s not found.\n", pgm);
        exit(1);
    }

    listing = stdout;

    fprintf(listing, "TINY COMPILATION: %s\n", pgm);

    #if NO_PARSE
    while (getToken() != ENDFILE);
    #else
    syntaxTree = parse();
    if (TraceParse) {
        fprintf(listing, "\nSyntax tree: \n");
        printTree(syntaxTree);
    }
    
    buildSymtab(syntaxTree);
    typeCheck(syntaxTree);
    
    #endif
    return 0;
}
