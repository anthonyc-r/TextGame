#ifndef _CREATURE_H
#define _CREATURE_H
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

struct mem;
struct ent;
struct map;
struct scrpt;
struct ctr;
struct sound;
enum dir_t;

struct ctr {
	int idx;
    struct vector2i position;
    int health;
	int tp;
	int inventory_size;
    char *name;
	char *desc;
    int hearing;
    struct ent *inventory;
	struct map *map;
	struct scrpt *script;
	struct mem *speech;
	struct mem *memory;
} *all_creatures, *player;


void creature_walk(struct ctr *creature, enum dir_t direction);
bool creature_walk_toward(struct ctr *creature, struct ctr *other);
struct ctr new_creature(char *name, char *desc, int health, int tp, size_t inventory_size);
struct ctr** creature_look(struct ctr *creature);
struct sound **creature_listen(struct ctr *creature);
struct sound *creature_consume_speech(struct ctr *creatuere);
#endif
