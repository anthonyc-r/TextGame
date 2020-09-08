/*
Copyright 2017-2020 Anthony Cohn-Richardby

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


#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char **
split_line(char *l, char split)
{
	int len = strlen(l) + 1;
	char *line = malloc(len);
	char **p;
	int ps = 0;
	int i;
	strncpy(line, l, len);
	for (i = 0; i < len; ++i) {
		if (line[i] == split)
			ps++;
	}
	p = malloc(sizeof (char*) * (ps + 2));
	ps = 0;
	p[0] = line;
	for (i = 0; i < len; ++i) {
		if (line[i] == split) {
			ps++;
			line[i] = '\0';
			p[ps] = &line[i+1];
		}
	}
	p[ps+1] = NULL;
	return p;
}

char *
read_line(FILE *fp) 
{
	char *line;
	int i = 0;
	int c;
	fpos_t pos;
	fgetpos(fp, &pos);

	while ((char)(c = fgetc(fp)) != '\n' && c != EOF)
		++i;
	if (c == EOF && i == 0)
		return NULL;
	fsetpos(fp, &pos);
	line = malloc(i + 1);
	i = 0;
	while ((line[i] = fgetc(fp)) != '\n' && c != EOF)
		++i;
	line[i] = '\0';
	return line;
}
