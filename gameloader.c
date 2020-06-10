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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "gameloader.h"
#include "entity.h"
#include "creature.h"
#include "ground.h"
#include "utility.h"
#include "sound.h"

void
load_game(char *filename)
{
	FILE *fp = fopen(filename, "r");
	char *line;
	int i = 0;
	all_entities = malloc(100 * sizeof (struct ent));
	all_creatures = malloc(100 * sizeof (struct ctr));
	all_grounds = malloc(100 * sizeof (struct grnd));
	all_sound_types = malloc(100 * sizeof (struct sound));
	while (*(line = read_line(fp)) != '\t') {
		printf("loading entity %d with data '%s'\n", i, line);
		char **splits = split_line(line, '\t');
		char *name = splits[0];
		char *icon = splits[1];
		char *weight = splits[2];
		char *size_class = splits[3];
		all_entities[i] = new_entity(name, "No description", *icon, intval(weight), entity_size(size_class));
		++i;
	}
	i = 0;
	while (*(line = read_line(fp)) != '\t') {
		printf("loading ground %d with data '%s'\n", i, line);
		char **splits = split_line(line, '\t');
		char *name = splits[0];
		char *icon = splits[1];
		all_grounds[i] = new_ground(name, *icon);
		++i;
	}
	i = 0;
	while (*(line = read_line(fp)) != '\t') {
		printf("loading creature %d with data '%s'\n", i, line);
		char **splits = split_line(line, '\t');
		char *name = splits[0];
		char *health = splits[1];
		char *tp = splits[2];
		char *inventory_size = splits[3];
		all_creatures[i] = new_creature(name, "No description", intval(health), intval(tp), (size_t) intval(inventory_size));
		++i;
	}
	i = 0;
	while (*(line = read_line(fp)) != '\t') {
		printf("loading sound %d with data '%s'\n", i, line);
		char **splits = split_line(line, '\t');
		char *id = splits[0];
		char *text = splits[1];
		all_sound_types[i].id = id;
		all_sound_types[i].text = text;
		++i;
	}
	all_sound_types_count = i;
}

int
intval(char *s)
{
	int len = strlen(s);
	int v = 0;
	for (int i = 0; i < len; ++i) {
		v += pow(10, len - i - 1) * ((int) s[i] - 48);
	}
	return v;
}
