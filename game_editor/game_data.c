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
#include <stdint.h>

#include "game_data.h"

static inline void 
cpyname(char *dst, const char *src, int max)
{
	strncpy(dst, src, max);
	dst[max - 1] = '\0';
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
		case SIZE_NONE:
			return "SIZE_NONE";
	}
}

static void
save_game_types(FILE *file, struct entity **entities, struct ground **grounds, struct creature **creatures)
{
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
	// sounds?
	fprintf(file, "\t\n");
}

static void
save_map(FILE *file, struct entity **entities, struct ground **grounds, struct creature **creatures, struct map *map)
{
	struct cell *cell;
	uint32_t index;
	fwrite(&map->width, sizeof (uint32_t), 1, file);
	fwrite(&map->height, sizeof (uint32_t), 1, file);
	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			cell = map_get_cell(map, x, y);
			index = (cell->entities != NULL) ? cell->entities->index 
				: DAT_EMPTY_INDEX;
			printf("index %ud\n", index);
			fwrite(&index, sizeof (uint32_t), 1, file);
			index = (cell->ground != NULL) ? cell->ground->index 
				: DAT_EMPTY_INDEX;
			printf("index %ud\n", index);
			fwrite(&index, sizeof (uint32_t), 1, file);
			index = (cell->creature != NULL) ? cell->creature->index 
				: DAT_EMPTY_INDEX;
			printf("index %ud\n", index);
			fwrite(&index, sizeof (uint32_t), 1, file);
		}
	}
}

void 
save_game_data(char *outpath, struct entity **entities, struct ground **grounds, struct creature **creatures, struct map *map)
{
	FILE *file = fopen(outpath, "w+");
	save_game_types(file, entities, grounds, creatures);
	fprintf(file, DAT_MAP_SEPARATOR"\n");
	save_map(file, entities, grounds, creatures, map);
	fprintf(file, DAT_END"\n");
	fclose(file);
}

enum size_type 
entity_size(const char *string)
{
	if (strncmp(string, "SIZE_HUGE", 9) == 0) {
		return SIZE_HUGE;
	} else if (strncmp(string, "SIZE_LARGE", 10) == 0) {
		return SIZE_LARGE;
	} else if (strncmp(string, "SIZE_SMALL", 10) == 0) {
		return SIZE_SMALL;
	} else if (strncmp(string, "SIZE_TINY", 9) == 0) {
		return SIZE_TINY;
	} else {
		return SIZE_NONE;
	}
}


static void
load_game_types(FILE *file, struct entity ***edest, struct ground ***gdest, struct creature ***cdest)
{
	int ecount, gcount, ccount, scount;
	char linebuff[MAX_LINE];	
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
	// sounds
	while (*(line = fgets(linebuff, MAX_LINE, file)) != '\t')
		;
	
	*edest = ents;
	*gdest = grounds;
	*cdest = creatures;
}

static struct map *
load_game_map(FILE *file, struct entity **ents, struct ground **grounds, struct creature **creatures)
{
	int width, height;
	fread(&width, sizeof (uint32_t), 1, file);
	fread(&height, sizeof (uint32_t), 1, file);
	struct map *map = new_map(width, height);
	printf("loading game map of size %d, %d\n", width, height);
	uint32_t index;
	struct cell *cell;
	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			cell = map_get_cell(map, x, y);
			fread(&index, sizeof (uint32_t), 1, file);
			printf("fread index: %ud\n", index);
			if (index != DAT_EMPTY_INDEX) {
				cell->entities = ents[index];
			}
			fread(&index, sizeof (uint32_t), 1, file);
			printf("fread index: %ud\n", index);
			if (index != DAT_EMPTY_INDEX) {
				cell->ground = grounds[index];
			}
			fread(&index, sizeof (uint32_t), 1, file);
			printf("fread index: %ud\n", index);
			if (index != DAT_EMPTY_INDEX) {
				cell->creature = creatures[index];
			}
		}
	}
	return map;
}

struct map * 
load_game_data(char *inpath, struct entity ***edest, struct ground ***gdest, struct creature ***cdest)
{
	char buf[MAX_LINE];
	FILE *file = fopen(inpath, "r");
	load_game_types(file, edest, gdest, cdest);
	// Separator
	fgets(buf, MAX_LINE, file);
	struct map *map = load_game_map(file, *edest, *gdest, *cdest);
	fclose(file);
	return map;
}

struct map *
load_old_map_data(char *inpath, struct ground **grounds, struct entity **entities, struct creature **creatures)
{
	int width, height;
	char buf[255];
	FILE *file = fopen(inpath, "r");
	fgets(buf, 255, file);
	fgets(buf, 255, file);
	int delim = -1;
	while (buf[++delim] != ' ')
		;
	buf[delim] = '\0';
	width = (int)strtol(buf, NULL, 10);
	height = (int)strtol(buf + delim + 1, NULL, 10);
	printf("loading map width, %d, height %d\n", width, height);
	struct map *map = new_map(width, height);
	fgets(buf, 255, file);
	char groundi, entityi, creaturei;
	struct creature *creature;
	struct entity *entity;
	struct ground *ground;
	struct cell *cell;
	int index;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			cell = map_get_cell(map, x, y);
			index = (int)fgetc(file);
			if (index > 0 && index != 255) {
				printf("load ground %d\n", index);
				cell->ground = grounds[index];
			}
			index = (int)fgetc(file);
			if (index > 0 && index != 255) {
				printf("load entity %d\n", index);
				cell->entities = entities[index];
			}
			index = (int)fgetc(file);
			if (index > 0 && index != 255) {
				printf("load creature %d\n", index);
				cell->creature = creatures[index];
			}
		}
	}
	return map;
}

void 
load_old_game_data(char *inpath, struct entity ***edest, struct ground ***gdest, struct creature ***cdest)
{
	FILE *file = fopen(inpath, "r");
	load_game_types(file, edest, gdest, cdest);
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

void
copy_name(char *dest, const char *src)
{
	cpyname(dest, src, MAX_NAME);
}

void
copy_desc(char *dest, const char *src)
{
	cpyname(dest, src, MAX_DESC);
}

