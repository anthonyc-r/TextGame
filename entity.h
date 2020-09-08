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

#ifndef _ENTITY_H
#define _ENTITY_H

#include<stdbool.h>

#include "script.h"
#include "vector.h"

struct map;
struct mem;
struct ctr;

enum size_type {
	SIZE_TINY,
	SIZE_SMALL,
	SIZE_LARGE,
	SIZE_HUGE
};

struct ent {
	int idx;
    char *name;
	char *desc;
	char icon;
    int weight;
    struct vector2i position;
    struct map *map;
	enum size_type size;
	struct mem *memory;
	struct scrpt *script;
	// list
	struct ent *next;
	struct ent *prev;
} *all_entities;

struct ent* init_entity(struct ent *entity, char *name, char *desc, char icon, int weight, enum size_type size);
struct ent new_entity(char *name, char *desc, char icon, int weight, enum size_type size);
int entity_blocking(struct ent *entity);
enum size_type entity_size(char *string);
bool entity_use_creature(struct ent *entity, struct ctr *creature);
#endif
