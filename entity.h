#ifndef _ENTITY_H
#define _ENTITY_H

#include<stdbool.h>

#include "vector.h"

struct map;
struct mem;

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
	// list
	struct ent *next;
	struct ent *prev;
} *all_entities;

struct ent* init_entity(struct ent *entity, char *name, char *desc, char icon, int weight, enum size_type size);
struct ent new_entity(char *name, char *desc, char icon, int weight, enum size_type size);
int entity_blocking(struct ent *entity);
enum size_type entity_size(char *string);
#endif
