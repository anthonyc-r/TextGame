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

#ifndef _MEMORY_H
#define _MEMORY_H
#include <stdbool.h>

#include "sound.h"

struct ctr;
struct map;
struct scrpt;


#define MEMORY_SIZE 100

/*
	May contain one of many data types depending on what is committed to memory...
*/
enum mem_type {
	MEM_TYPE_NONE, MEM_TYPE_CREATURE, MEM_TYPE_STRING, MEM_TYPE_SOUND
};

struct mem_data_creature {
	int idx;
	char *name;
};

struct mem_data_sound {
	int entity_source_idx;
	int creature_source_idx;
	enum sound_source_t source_t;
	char *text;
};

struct mem_data_string {
    char *chars;
};

union mem_data {
	struct mem_data_creature creature;
	struct mem_data_sound sound;
	struct mem_data_string string;
	void *address;
};

struct mem {
	union mem_data data;
	enum mem_type type;
	struct mem *next;
};

struct mem *new_memory();
struct mem *memory_copy(struct mem *other);
void deinit_memory(struct mem *memory);

void memory_set_string(struct mem *memory, char *string);
void memory_set_creature(struct mem *memory, struct ctr *creature);
void memory_set_sound(struct mem *memory, struct sound *sound);

void memory_push(struct mem **top, struct mem *addition);
void memory_push_copy(struct mem **top, struct mem *addition);
struct mem* memory_pop(struct mem **top);

bool memory_eq_string(struct mem *memory, char *string);
bool memory_eq_ctr(struct mem *memory, struct ctr *creature);
bool memory_is_none(struct mem *memory);
//bool memory_eq(struct mem *memory1, struct mem *memory2);

void script_remember_creatures(struct scrpt *script, struct ctr **creatures);
struct ctr *memory_active_creature(struct mem *memory, struct map *map);

#endif
