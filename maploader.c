
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "maploader.h"
#include "map.h"
#include "creature.h"
#include "script.h"
#include "vector.h"
#include "entity.h"
#include "debug.h"

struct map *
load_map(char *filename)
{
	printf("loading map\n");
	FILE *fp = fopen(filename, "rb");
	printf("opened map file rb\n");
	int width;
	int height;
	if (read_metadata(fp, &width, &height)) {
		printf("Invalid map file\n");
		return NULL;
	} else {
		printf("Sucessfully loaded metadata\n");
	}
	
	printf("Processed values: Map width: %d, height: %d.\n", width, height);
	fflush(stdout);
	struct map *map = init_map(width, height);
	printf("init_map done.\n");
	fflush(stdout);
	map->creatures = malloc(20 * sizeof(struct ctr));
	map->creature_count = 0;
	getc(fp);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
            DEBUG_PRINT(("Setting up cell, %d, %d\n", i, j));
			struct cell *current_cell = map->cells + ((i * width) + j);
			char ground = getc(fp);
			char entity = getc(fp) - 1;
			char creature = getc(fp) - 1;
			current_cell->ground = ground;
			if (creature >= 0) {
				struct scrpt *script = script_init("data_devel//gatekeeper_1.script");
				struct vector2i position = { j, i };
				insert_creature(map, &all_creatures[(int) creature], script, position);
			}
			if (entity >= 0) {
                struct vector2i position = { j, i };
                struct scrpt *script = script_init("data_devel//sword.script");
				insert_entity(map, all_entities[(int) entity], script, position);
			}
		}
	}
	map->creatures = realloc(map->creatures, (map->creature_count + 1) * sizeof(struct ctr));
	return map;
}

int
ascii_dec(char *ascii)
{
	printf("ascii_desc for string %s\n", ascii);
	fflush(stdout);
	int ret = 0;
	int len = strlen(ascii);
	
	for (int i = 0; i < len; ++i) {
		ret += pow(10, len - i - 1) * (ascii[i] - 48);
	}
	printf("ret ascii_desc %d\n", ret);
	return ret;
}
int
read_metadata(FILE *fp, int *width, int *height)
{
	char *magic = malloc(4);
	fgets(magic, 4, fp);
	if (!strcmp(magic, "P6")) {
		return -1;
	}
	free(magic);
	
	char line[21];
	char width_c[10];
	char height_c[10];
	fgets(line, 21, fp);
	printf("size and height line: %s\n", line);
	size_t i;
	size_t j;
	char c;
	for (i = 0; (c = line[i]) != ' '; ++i) {
		width_c[i] = c;
	}
	width_c[i] = '\0';
	j = ++i;
	for (i = 0; (c = line[i + j]) != '\n'; ++i) {
		height_c[i] = c;
	}
	height_c[i] = '\0';
	char skip[4];
	fgets(skip, 4, fp);
	
	printf("width: %s, height: %s\n", width_c, height_c);
	fflush(stdout);
	*width = ascii_dec(width_c);
	*height = ascii_dec(height_c);
	return 0;
}

