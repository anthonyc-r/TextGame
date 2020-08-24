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
load_map(FILE *file)
{
	printf("loading map\n");
	// Separator
	char buf[MAX_LINE];
	fgets(buf, MAX_LINE, file);
	
	int width, height;
	fread(&width, sizeof (uint32_t), 1, file);
	fread(&height, sizeof (uint32_t), 1, file);
	struct map *map = init_map(width, height);
	// TODO: - Don't hardcode this 20?!
	map->creatures = malloc(20 * sizeof(struct ctr));
	map->creature_count = 0;
	printf("loading game map of size %d, %d\n", width, height);
	uint32_t index;
	struct cell *current_cell;
	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
		 	current_cell = map->cells + ((y * width) + x);
			fread(&index, sizeof (uint32_t), 1, file);
			printf("fread index: %ud\n", index);
			if (index != DAT_EMPTY_INDEX) {
				struct vector2i position = { x, y };
				struct scrpt *script = script_init("data_devel//sword.script");
				insert_entity(map, all_entities[index], script, position);
			}
			fread(&index, sizeof (uint32_t), 1, file);
			printf("fread index: %ud\n", index);
			if (index != DAT_EMPTY_INDEX) {
				current_cell->ground = index;
			} else {
				current_cell->ground = -1;
			}
			fread(&index, sizeof (uint32_t), 1, file);
			printf("fread index: %ud\n", index);
			if (index != DAT_EMPTY_INDEX) {
				struct scrpt *script = script_init("data_devel//gatekeeper_1.script");
				struct vector2i position = { x, y };
				insert_creature(map, &all_creatures[index], script, position);
			}
		}
	}
	map->creatures = realloc(map->creatures, (map->creature_count + 1) * sizeof(struct ctr));
	return map;
}

