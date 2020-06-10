#ifndef _NARRATOR_H
#define _NARRATOR_H

#define MAX_NARRATOR_LINE 100

struct narrator {
	char narrator_line[MAX_NARRATOR_LINE];
	char **all_lines;
} main_narrator;

void narrate(struct narrator *narrator, char *string);
char *next_narration(struct narrator *narrator);

#endif
