//define DEBUG

#include "rouge.h"
#include "entity.h"
#include "memory.h"
#include "creature.h"
#include "map.h"
#include "vector.h"
#include "debug.h"
#include "sound.h"
#include "utility.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

bool
creature_walk_toward(struct ctr *creature, struct ctr *other)
{
	struct vector2i *pos_src = &creature->position;
	struct vector2i *pos_dest = &other->position;
	int dx = pos_dest->x - pos_src->x;
	int dy = pos_dest->y - pos_src->y;
	enum dir_t dir = NO_DIRECTION;
	if (dx > 0) {
		dir = EAST;
	} else if (dx < 0) {
		dir = WEST;
	} else if (dy > 0) {
		dir = SOUTH;
	} else if (dy < 0) {
		dir = NORTH;
	}
	creature_walk(creature, dir);
	if (abs(dx) < 3 && abs(dy) < 3)
		return true;
	else
		return false;
}

void
creature_walk(struct ctr *creature, enum dir_t direction)
{
	struct vector2i dest_pnt = creature->position;
	struct cell *current_cell;
	struct cell *dest_cell;
	struct ent *entity;
	bool blocked = false;
	
	switch (direction) {
		case NORTH:
			dest_pnt.y -= 1;
			break;
		case EAST:
			dest_pnt.x += 1;
			break;
		case SOUTH:
			dest_pnt.y += 1;
			break;
		case WEST:
			dest_pnt.x -= 1;
			break;
		default:
			break;
	}
	current_cell = get_cell(creature->map, &creature->position);
	dest_cell = get_cell(creature->map, &dest_pnt);
	if (!dest_cell || !current_cell) {
		DEBUG_PRINT(("Destination or current cell null.\n"));
		return;
	}
	
	//Void cells are unpassable by walking.
	if (dest_cell->ground == 0 || dest_cell->ground == -1) 
		blocked = true;

	//Check for blocking items.
	for (int i = 0; i < dest_cell->inventory_size; ++i) {
		entity = &dest_cell->inventory[i];
		if (entity_blocking(entity)) {
			blocked = true;
			break;
		}
	}
	if (!blocked) {
		current_cell->creature = NULL;
		dest_cell->creature = creature;
        creature->position = dest_pnt;
	}
}

struct ctr
new_creature(char *name, char *desc, int health, int tp, size_t inventory_max)
{
	struct ctr creature;
	creature.idx = -1;
	creature.speech = new_memory();
	creature.memory = new_memory();
	creature.name = name;
	creature.desc = desc;
	creature.health = health;
	creature.tp = tp;
	creature.inventory_max = inventory_max;
	creature.inventory_size = 0;
    creature.hearing = 5;
	creature.inventory = malloc(sizeof (struct ent*) * inventory_max);
    creature.script = NULL;
    creature.map = NULL;
	return creature;
}

struct ctr**
creature_look(struct ctr *creature)
{
	struct ctr **ret = malloc(sizeof (struct ctr*) * MAX_CREATURE_SIGHTINGS);
	int sightings = 0;
	float sight_radius = 5.0;
	float radius_incr = 0.2;
	float radius_start = 0.3;
	int angle_count = (int) 2.0 * M_PI / CREATURE_LOOK_ANGLE;
	struct vector2f origin = vector2f_i(creature->position);
	// Look along vectors pointing outwards from the origin for creatures
	for (int i = 0; i < angle_count; i++) {
		float angle = i * CREATURE_LOOK_ANGLE;
		struct vector2f dest = vector2f_angle(origin, sight_radius, angle);
		for (float j = radius_start; j < 1.0; j += radius_incr) { 
			struct vector2i p = vector2f_round(vector2f_along(origin, dest, j));
			struct cell *c = get_cell(creature->map, &p);
			if (c != NULL && c->creature != NULL) {
				if (sightings < 1 || ret[sightings - 1] != c->creature) {
					DEBUG_PRINT(("Found creature name: %s\n", c->creature->name));
					ret[sightings++] = c->creature;
				}
				break;
			}
		}
		if (sightings >= MAX_CREATURE_SIGHTINGS)
			break;
	}
	ret = realloc(ret, sizeof(struct ctr*) * sightings + 1);
	ret[sightings] = NULL;
	DEBUG_PRINT(("Found %d creatures\n", sightings));
	return ret;
}

void
creature_say_str(struct ctr *creature, char *str)
{
	struct mem *memory = new_memory();
	memory_set_string(memory, str);
	memory_push(&creature->speech, memory);
}

struct sound *
creature_consume_speech(struct ctr *creature)
{
    char *text = NULL;
    struct mem *speech = creature->speech;
    char *strings[MAX_SPEECH_SIZE];
    int lengths[MAX_SPEECH_SIZE];
    int len = 0;
    int i = 0;
    do {
        if (speech->type != MEM_TYPE_STRING)
            continue;
        strings[i] = speech->data.string.chars;
        lengths[i] = strlen(strings[i]);
        len += lengths[i];
        DEBUG_PRINT(("Found string %s, length: %d\n", strings[i], lengths[i]));
        i++;
    } while (i < MAX_SPEECH_SIZE && (speech = speech->next));
    if (i == 0)
        return NULL;
    DEBUG_PRINT(("Found strings, count: %d, length: %d\n", i, len));
    text = malloc(len + 1);
    text[len] = 0;
    for (i--, len = 0; i >= 0; i--) {
        memcpy(&text[len], strings[i], lengths[i]);
        len += lengths[i];
    }
    while ((speech = memory_pop(&creature->speech))) {
        deinit_memory(speech);
        free(speech);
    }
    struct sound_t *sound_type = malloc(sizeof(struct sound_t));
    sound_type->id = "";
    sound_type->text = text;
    return sound_creature(sound_type, creature);
}

void
clip_vec(struct map *map, struct vector2i *v)
{	
	if (v->x < 0)
		v->x = 0;
	else if (v->x >= map->width)
		v->x = map->width - 1;
	
	if (v->y < 0)
		v->y = 0;
	else if (v->y >= map->height)
		v->y = map->height - 1;
}

// Returns an array of references to sounds. The map struct owns these sounds.
struct sound **
creature_listen(struct ctr *creature)
{
    struct sound **sounds = malloc(((creature->hearing + 1) * (creature->hearing + 1) + 1) * sizeof(struct sound*));
    struct map *map = creature->map;
    struct vector2i topleft = {
        creature->position.x - creature->hearing,
        creature->position.y - creature->hearing
    };
	clip_vec(map, &topleft);
    struct vector2i bottomright = {
        creature->position.x + creature->hearing,
        creature->position.y + creature->hearing
    };
	clip_vec(map, &bottomright);
    int count = 0;
    for (int i = topleft.x; i <= bottomright.x; i++) {
        for (int j = topleft.y; j <= bottomright.y; j++) {
            struct vector2i pos = { i, j };
            struct cell *cell = get_cell(map, &pos);
            if (cell->sound)
                sounds[count++] = cell->sound;
        }
    }
    sounds = realloc(sounds, (count + 1) * sizeof(struct sound*));
    sounds[count] = NULL;
    return sounds;
}

bool
creature_take_entity(struct ctr *creature, struct ent *item)
{
	if (creature->inventory_size >= creature->inventory_max) {
		// inventory full
		return false;
	}
	creature->inventory[(creature->inventory_size)++] = item;
	return true;
}

// Put all the items nearby into dst if non null & return number of items nearby
int
creature_search_items(struct ctr *creature, struct ent **dst)
{
	int prange = 1;
	int count = 0;
	struct map *map = creature->map;
	struct vector2i topleft = {
		creature->position.x - prange,
		creature->position.y - prange
	};
	struct vector2i bottomright = {
		creature->position.x + prange,
		creature->position.y + prange
	};
	clip_vec(map, &topleft);
	clip_vec(map, &bottomright);
	for (int i = topleft.x; i <= bottomright.x; i++) {
		for (int j = topleft.y; j <= bottomright.y; j++) {
			struct vector2i pos = { i, j };
			struct cell *cell = get_cell(map, &pos);
			if (dst != NULL) {
				struct ent *item = cell->inventory;
				while (item != NULL) {
					dst[count++] = item;
					item = item->next;
				}
			} else {
				count += cell->inventory_size;
			}
		}
	}
	return count;
}

