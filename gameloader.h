#ifndef _GAMELOADER_H
#define _GAMELOADER_H
#include <stdio.h>

void load_game(char *filename);
int read_item(char *line, char *name, char *icon, int *weight);
int read_cell(char *line, char *name, char *icon);
int read_creature(char *line, char *name, int *health, int *tp, int *inventory, char *desc);
int count_lines(FILE *fp);
int intval(char *c);
#endif
