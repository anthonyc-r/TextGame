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


#ifndef _MAP_H
#define _MAP_H

//top left == 0,0
#include<stdlib.h>
#include<stdbool.h>

struct scrpt;
struct sound;
struct wnw;
struct map;
struct cell;
struct vector2i;
struct ent;
struct sound;
enum dir_t;

static const char CELL_SPACE_ICON = '.';


struct map {
	int width;
	int height;
	int size;
	int creature_count;
	struct ctr *creatures;
	int sound_count;
	struct sound **sounds;
	struct cell *cells;
} *active_map;

struct cell {
    int ground;
    
    struct cell *north;
    struct cell *east;
    struct cell *south;
    struct cell *west;
    
	int inventory_size;
    struct ent *inventory;
    struct ctr *creature;
    struct sound *sound;
};

struct map *init_map(int width, int height);
void destroy_map(struct map *map);
void render_map(struct map *map, struct wnw *window, struct vector2i *from, struct vector2i *to);
void insert_entity(struct map *map, struct ent entity, struct scrpt *script, struct vector2i position);
void map_remove_entity(struct ent *entity);
void insert_sound(struct map *map, struct sound *sound);
void clear_sounds(struct map *map);
struct ctr *insert_creature(struct map *map, struct ctr *creature, struct scrpt *script, struct vector2i position);
void map_consume_speech(struct ctr *creature);
char cell_to_char(struct cell *cel);
struct cell *dir_to_cell(struct cell *cel, enum dir_t direction);
struct cell *get_cell(struct map *map, struct vector2i *point);
bool cell_blocks_sight(struct cell *c);
bool cell_blocks_movement(struct cell *c);
#endif
