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
#include <stdio.h>

#include "game_data.h"

static inline void 
cpyname(char *dst, char *src, int max)
{
	strncpy(dst, src, max);
	dst[MAX_NAME - 1] = '\0';
}

struct entity *
new_entity(char *name, char *desc, char icon, int weight, enum size_type size_class)
{
	struct entity *entity = malloc(sizeof(struct entity));
	cpyname(entity->name, name, MAX_NAME);
	entity->weight = weight;
	cpyname(entity->desc, desc, MAX_DESC);
	entity->icon = icon;
	entity->size_class = size_class;
	return entity;
}

struct ground *
new_ground(char *name, char icon)
{
	struct ground *ground = malloc(sizeof(struct ground));
	cpyname(ground->name, name, MAX_NAME);
	ground->icon = icon;
	return ground;
}


struct creature *
new_creature(char *name, char *desc, int health, int tp, int inventory_size)
{
	struct creature *creature = malloc(sizeof(struct creature));
	cpyname(creature->name, name, MAX_NAME);
	creature->health = health;
	cpyname(creature->desc, desc, MAX_DESC);
	creature->tp = tp;
	creature->inventory_size = inventory_size;
	return creature;
}


char *
size_type_str(enum size_type type)
{
	switch (type) {
		case SIZE_HUGE:
			return "SIZE_HUGE";
		case SIZE_LARGE:
			return "SIZE_LARGE";
		case SIZE_SMALL:
			return "SIZE_SMALL";
		case SIZE_TINY:
			return "SIZE_TINY";
	}
}
void 
save_game_data(char *outpath, struct entity **entities, struct ground **grounds, struct creature **creatures)
{
	FILE *file = fopen(outpath, "w+");
	struct entity *ent;
	struct creature *creat;
	struct ground *ground;
	while ((ent = *entities) != NULL) {
		fprintf(file, "%s\t%c\t%d\t%s\n",
			ent->name,
			ent->icon,
			ent->weight,
			size_type_str(ent->size_class));
		entities++;
	}
	fprintf(file, "\t\n");
	while ((ground = *grounds) != NULL) {
		fprintf(file, "%s\t%c\n",
			ground->name,
			ground->icon);
		grounds++;
	}
	fprintf(file, "\t\n");
	while ((creat = *creatures) != NULL) {
		fprintf(file, "%s\t%d\t%d\t%d\t%s\n",
			creat->name,
			creat->health,
			creat->tp,
			creat->inventory_size,
			creat->desc);
		creatures++;
	}
	fprintf(file, "\t\n");
	fprintf(file, "\t\n");
	fclose(file);
}

enum size_type 
entity_size(char *string)
{
	int len = strlen(string);
	if (strncmp(string, "SIZE_HUGE", len) == 0) {
		return SIZE_HUGE;
	} else if (strncmp(string, "SIZE_LARGE", len) == 0) {
		return SIZE_LARGE;
	} else if (strncmp(string, "SIZE_SMALL", len) == 0) {
		return SIZE_SMALL;
	} else if (strncmp(string, "SIZE_TINY", len) == 0) {
		return SIZE_TINY;
	} else {
		return SIZE_TINY;
	}
}
void 
load_game_data(char *inpath, struct entity ***edest, struct ground ***gdest, struct creature ***cdest)
{
	int ecount, gcount, ccount, scount;
	char linebuff[MAX_LINE];
	FILE *file = fopen(inpath, "r");
	
	ecount = 0;
	ccount = 0;
	gcount = 0;
	scount = 0;
	while (*fgets(linebuff, MAX_LINE, file) != '\t')
		ecount++;
	while (*fgets(linebuff, MAX_LINE, file) != '\t')
		gcount++;
	while (*fgets(linebuff, MAX_LINE, file) != '\t')
		ccount++;
	while (*fgets(linebuff, MAX_LINE, file) != '\t')
		scount++;
	
	printf("ecount: %d, gcount: %d, ccount: %d\n", ecount, gcount, ccount);
	
	struct entity **ents = malloc(ecount * sizeof(struct entity*) + 1);
	struct ground **grounds = malloc(gcount * sizeof(struct ground*) + 1);
	struct creature **creatures = malloc(ccount * sizeof(struct creatures*) + 1);

	fseek(file, 0, SEEK_SET);
	int i = 0;
	char *line;
	while (*(line = fgets(linebuff, MAX_LINE, file)) != '\t') {
		printf("loading entity %d with data '%s'\n", i, line);
		char *name = strtok(line, "\t");
		char *icon = strtok(NULL, "\t");
		int weight = (int)strtol(strtok(NULL, "\t"), NULL, 10);
		enum size_type size = entity_size(strtok(NULL, "\t"));
		ents[i++] = new_entity(name, "No description", *icon, weight, size);
	}
	ents[i] = NULL;
	i = 0;
	while (*(line = fgets(linebuff, MAX_LINE, file)) != '\t') {
		printf("loading ground %d with data '%s'\n", i, line);
		char *name = strtok(line, "\t");
		char *icon = strtok(NULL, "\t");
		grounds[i++] = new_ground(name, *icon);
	}
	grounds[i] = NULL;
	i = 0;
	while (*(line = fgets(linebuff, MAX_LINE, file)) != '\t') {
		printf("loading creature %d with data '%s'\n", i, line);
		char *name = strtok(line, "\t");
		int health = (int)strtol(strtok(NULL, "\t"), NULL, 10);
		int tp = (int)strtol(strtok(NULL, "\t"), NULL, 10);
		int inventory_size = (int)strtol(strtok(NULL, "\t"), NULL, 10);
		creatures[i++] = new_creature(name, "No description", health, tp, inventory_size);
	}
	creatures[i] = NULL;
	
	*edest = ents;
	*gdest = grounds;
	*cdest = creatures;
	fclose(file);
}

struct map *
new_map(int width, int height) 
{
	struct map *map = malloc(sizeof (struct map));
	map->width = width;
	map->height = height;
	map->cells = malloc(width * height * sizeof (struct cell));
	struct cell *cell;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			cell = map->cells + (height * y + x);
			cell->x = x;
			cell->y = y;
			cell->ground = NULL;
			cell->entity_count = 0;
			cell->entities = NULL;
			cell->creature = NULL;
		}
	}
	return map;
}

void 
destroy_map(struct map *map)
{
	free(map->cells);
	free(map);
}

struct cell *
map_get_cell(struct map *map, int x, int y)
{
	return map->cells + (y * map->height + x);
}


