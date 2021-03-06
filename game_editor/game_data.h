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

#ifndef _ENTITY_H
#define _ENTITY_H

#define MAX_NAME 20
#define MAX_DESC 100
#define MAX_LINE 200
#define DAT_MAP_SEPARATOR "\a\t\f"
#define DAT_END "\f\t\a"
// Keep in sync with the main game.
#define DAT_EMPTY_INDEX 0xFFFFFFFF

enum size_type {
	SIZE_NONE,
	SIZE_TINY,
	SIZE_SMALL,
	SIZE_LARGE,
	SIZE_HUGE
};
struct entity {
	char name[MAX_NAME];
	char desc[MAX_DESC];
	char icon;
	int weight;
	enum size_type size_class;
	int index;
};
struct creature {
	char name[MAX_NAME];
	char desc[MAX_DESC];
	int health;
	int tp;
	int inventory_size;
	int index;
};
struct ground {
	char name[MAX_NAME];
	char icon;
	int index;
};
struct cell {
	int x;
	int y;
	struct ground *ground;
	int entity_count;
	struct entity *entities;
	struct creature *creature;
};
struct map {
	int width, height;
	struct cell *cells;
};

struct entity *new_entity(char *name, char *desc, char icon, int weight, enum size_type size_class);
struct ground *new_ground(char *name, char icon);
struct creature *new_creature(char *name, char *desc, int health, int tp, int inventory_size);

// MAP MANAGEMENT
struct map *new_map(int width, int height);
void destroy_map(struct map *map);
struct cell *map_get_cell(struct map *map, int x, int y);


// SAVE / LOAD Expects null terminated arrays
void save_game_data(char *outpath, struct entity **entities, struct ground **grounds, struct creature **creatures, struct map *map);
// Allocates memory
struct map *load_game_data(char *inpath, struct entity ***edest, struct ground ***gdest, struct creature ***cdest);
// Allocates memory, map needs destroying after...
struct map *load_old_map_data(char *inpath, struct ground **grounds, struct entity **entities, struct creature **creatures);
void load_old_game_data(char *inpath, struct entity ***edest, struct ground ***gdest, struct creature ***cdest);

// UTIL
enum size_type entity_size(const char *string);
char *size_type_str(enum size_type type);
void copy_name(char *dest, const char *src);
void copy_desc(char *dest, const char *src);
#endif
