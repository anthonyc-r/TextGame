#ifndef _CREATURE_H
#define _CREATURE_H
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

#define SPEECH_SIZE 100
#define MAX_SPEECH_SIZE 20
#define CREATURE_LOOK_ANGLE 0.1
#define MAX_CREATURE_SIGHTINGS 5

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
	int inventory_max;
    char *name;
	char *desc;
    int hearing;
    struct ent **inventory;
	struct map *map;
	struct scrpt *script;
	struct mem *speech;
	struct mem *memory;
} *all_creatures, *player;


void creature_walk(struct ctr *creature, enum dir_t direction);
bool creature_walk_toward(struct ctr *creature, struct ctr *other);
struct ctr new_creature(char *name, char *desc, int health, int tp, size_t inventory_max);
struct ctr** creature_look(struct ctr *creature);
struct sound **creature_listen(struct ctr *creature);
void creature_say_str(struct ctr *creature, char *str);
bool creature_take_entity(struct ctr *creature, struct ent *item);
int creature_search_items(struct ctr *creature, struct ent **dst);

#endif
