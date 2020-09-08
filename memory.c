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

//define DEBUG

#include "memory.h"
#include "creature.h"
#include "utility.h"
#include "debug.h"
#include "map.h"
#include "entity.h"
#include "memory.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct mem*
new_memory()
{
	struct mem *ret = calloc(1, sizeof(struct mem));
	return ret;
}

struct mem *
memory_copy(struct mem *other)
{
    struct mem *memory = new_memory();
    memory->type = other->type;
    if (other->type == MEM_TYPE_STRING) {
        memory->data.string.chars = malloc(strlen(other->data.string.chars));
        strcpy(memory->data.string.chars, other->data.string.chars);
    } else if (other->type == MEM_TYPE_SOUND) {
        memory->data.sound.text = malloc(strlen(other->data.sound.text));
        strcpy(memory->data.sound.text, other->data.sound.text);
    } else if (other->type == MEM_TYPE_CREATURE) {
        memory->data.creature.name = malloc(strlen(other->data.creature.name));
        strcpy(memory->data.creature.name, other->data.creature.name);
    }
    return memory;
}

void deinit_memory(struct mem *memory)
{
	if (memory->type == MEM_TYPE_STRING)
		free(memory->data.string.chars);
	else if (memory->type == MEM_TYPE_SOUND)
		free(memory->data.sound.text);
	else if (memory->type == MEM_TYPE_CREATURE)
		free(memory->data.creature.name);
}

void
memory_set_string(struct mem *memory, char *string)
{
	DEBUG_PRINT(("mem set string\n"));
	deinit_memory(memory);
	memory->type = MEM_TYPE_STRING;
	int len = strlen(string) + 1;
	memory->data.string.chars = malloc(len);
	strncpy(memory->data.string.chars, string, len);
}

void
memory_set_creature(struct mem *memory, struct ctr *creature)
{	
	DEBUG_PRINT(("mem set creature\n"));
	deinit_memory(memory);
	int len = strlen(creature->name) + 1;
	memory->type = MEM_TYPE_CREATURE;
	memory->data.creature.idx = creature->idx;
	memory->data.creature.name = malloc(len);
	strncpy(memory->data.creature.name, creature->name, len);
}

void
memory_set_sound(struct mem *memory, struct sound *sound)
{
	DEBUG_PRINT(("mem set speech\n"));
	deinit_memory(memory);
	int len = strlen(sound->type->text) + 1;
	memory->type = MEM_TYPE_SOUND;
	if (sound->source_t == SOUND_SOURCE_CREATURE) {
		memory->data.sound.creature_source_idx = sound->source.creature->idx;
	} else if (sound->source_t == SOUND_SOURCE_ENTITY) {
		memory->data.sound.entity_source_idx = sound->source.entity->idx;
	}
	memory->data.sound.text = malloc(len);
	strncpy(memory->data.sound.text, sound->type->text, len);
	memory->data.sound.source_t = sound->source_t;
}

void
memory_push(struct mem **top, struct mem *addition)
{
	addition->next = *top;
	*top = addition;
}

void
memory_push_copy(struct mem **top, struct mem *addition)
{
    struct mem *copy = memory_copy(addition);
    memory_push(top, copy);
}

struct mem*
memory_pop(struct mem **top)
{
	if ((*top)->next == NULL) {
		return NULL;
	}
	struct mem* ret = *top;
	*top = ret->next;
	return ret;
}

bool
memory_eq_string(struct mem *memory, char *string)
{
	switch (memory->type) {
		case MEM_TYPE_CREATURE:
			return strcmp(memory->data.creature.name, string) == 0;
			break;
		case MEM_TYPE_STRING:
			return strcmp(memory->data.string.chars, string) == 0;
			break;
		case MEM_TYPE_SOUND:
			return strcmp(memory->data.sound.text, string) == 0;
			break;
		case MEM_TYPE_NONE:
			return false;
			break;
	}
	return false;
}

bool
memory_eq_ctr(struct mem *memory, struct ctr *creature)
{
	if (memory->type != MEM_TYPE_CREATURE)
		return false;
	return memory->data.creature.idx == creature->idx;
}

bool 
memory_is_none(struct mem *memory)
{
	return memory->type == MEM_TYPE_NONE;
}

struct ctr *
memory_active_creature(struct mem *memory, struct map *map)
{
	if (memory->type != MEM_TYPE_CREATURE) {
		DEBUG_PRINT(("Invalid memory type for creature\n"));
		return NULL;
	}
	int idx = memory->data.creature.idx;
	DEBUG_PRINT(("Getting active creature at index: %d\n", idx));
	return &map->creatures[idx];
}
