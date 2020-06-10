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
