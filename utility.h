#ifndef _UTILITY_H
#define _UTILITY_H
#include <stdio.h>
#include <stdbool.h>

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

char ** split_line(char *l, char split);
char *read_line(FILE *fp);
#endif
