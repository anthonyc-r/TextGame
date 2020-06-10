/*
Copyright 2020 Anthony Cohn-Richardby

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _NARRATOR_H
#define _NARRATOR_H

#define MAX_NARRATOR_LINE 100

struct narrator_ll {
	char *line;
	struct narrator_ll *next;
};

struct narrator {
	char current_line[MAX_NARRATOR_LINE];
	struct narrator_ll *list_head;
	struct narrator_ll *list_tail;
} main_narrator;

void narrate(struct narrator *narrator, char *string);
char *next_narration(struct narrator *narrator);

#endif
