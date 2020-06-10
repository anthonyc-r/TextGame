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
#include <string.h>
#include <stdlib.h>

#include "narrator.h"
#include "utility.h"

void
narrate(struct narrator *narrator, char *string)
{
	int len = MIN(strlen(string), MAX_NARRATOR_LINE);
	char *copy = malloc(len);
	strncpy(copy, string, MAX_NARRATOR_LINE);
	copy[len - 1] = '\0';
	struct narrator_ll *ll = malloc(sizeof(struct narrator_ll));
	ll->line = copy;
	ll->next = NULL;
	if (narrator->list_head == NULL) {
		narrator->list_head = ll;
		narrator->list_tail = ll;
	} else {
		narrator->list_tail->next = ll;
		narrator->list_head = ll;
	}
}

char *
next_narration(struct narrator *narrator)
{
	if (narrator->list_head == NULL)
		return NULL;
	struct narrator_ll *ll = narrator->list_head;	
	strncpy(narrator->current_line, ll->line, MAX_NARRATOR_LINE);
	narrator->list_head = ll->next;
	free(ll->line);
	free(ll);
	return narrator->current_line;
}

