#include "globals.h"
#include "util.h"
#include "scan.h"

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

    while (getToken() != ENDFILE);

    return 0;
}