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

enum equip_location {
	EQUIP_LOCATION_WEAPON,
	EQUIP_LOCATION_BODY,
	EQUIP_LOCATION_HEAD,
	EQUIP_LOCATION_LEGS,
	EQUIP_LOCATION_FEET,
	_EQUIP_LOCATION_MAX
};

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
	struct ent *equipment[_EQUIP_LOCATION_MAX];
} *all_creatures, *player;


void creature_walk(struct ctr *creature, enum dir_t direction);
bool creature_walk_toward(struct ctr *creature, struct ctr *other);
struct ctr new_creature(char *name, char *desc, int health, int tp, size_t inventory_max);
struct ctr** creature_look(struct ctr *creature);
struct sound **creature_listen(struct ctr *creature);
void creature_say_str(struct ctr *creature, char *str);
bool creature_take_entity(struct ctr *creature, struct ent *item);
int creature_search_items(struct ctr *creature, struct ent **dst);
bool creature_equip(struct ctr *creature, struct ent *item, enum equip_location location);
void creature_unequip(struct ctr *creature, enum equip_location location);
enum equip_location creature_equip_location(struct ctr *creature, struct ent *item);
bool creature_attack(struct ctr *creature, struct ctr *target);

#endif
