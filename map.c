//define DEBUG

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "debug.h"
#include "map.h"
#include "vector.h"
#include "window.h"
#include "entity.h"
#include "rouge.h"
#include "ground.h"
#include "script.h"
#include "creature.h"
#include "memory.h"

#define SOUND_REALLOC_STEP 10

struct map *
init_map(int width, int height)
{
	struct map *map = malloc(sizeof(struct map));
    map->width = width;
    map->height = height;
    map->size = width * height;
    map->cells = malloc(map->size * sizeof(struct cell));
    DEBUG_PRINT(("Allocated map memory\n"));
    struct vector2i c_point = {0, 0};
    struct cell *c_cell;
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            c_point.x = j; c_point.y = i;
            c_cell = get_cell(map, &c_point);
            
            c_point.x = j;     c_point.y = i + 1;
            c_cell->north = get_cell(map, &c_point);
            
            c_point.x = j + 1; c_point.y = i;
            c_cell->east = get_cell(map, &c_point);
            
            c_point.x = j;     c_point.y = i - 1;
            c_cell->south = get_cell(map, &c_point);
            
            c_point.x = j - 1; c_point.y = i;
            c_cell->west = get_cell(map, &c_point);
            
            c_cell->ground = 0;
			c_cell->inventory_size = 0;
            c_cell->inventory = NULL;
            c_cell->creature = NULL;
            c_cell->sound = NULL;
		}
    }
	return map;
}

struct cell *
get_cell(struct map *map, struct vector2i *point)
{
    if (point->x >= 0 && point->x < map->width && point->y >= 0 && point->y < map->height) {
		return map->cells + (point->y * map->height + point->x);
    }
    else {
        return NULL;
    }
}

void
render_map(struct map *map, struct wnw *window, struct vector2i *from, struct vector2i *to)
{
    char *data = window->data;
    
	int index = 0;
	struct vector2i c_pnt;
	for (int i = from->y; i < to->y; ++i) {
		for (int j = from->x; j < to->x; ++j) {
			c_pnt.x = j; c_pnt.y = i;
			data[index] = cell_to_char(get_cell(map, &c_pnt));
			++index;
		}
		data[index] = '\n';
		++index;
	}
	data[index] = '\0';
}

struct cell *
dir_to_cell(struct cell *cel, enum dir_t direction)
{
	struct cell *rtn_cell = NULL;
	
	switch (direction) {
		case NORTH:
			rtn_cell = cel->north;
			break;
		case EAST:
			rtn_cell = cel->east;
			break;
		case SOUTH:
			rtn_cell = cel->south;
			break;
		case WEST:
			rtn_cell = cel->west;
			break;
		default:
			break;
	}
	
	return rtn_cell;
}

void 
insert_sound(struct map *map, struct sound *sound)
{
    DEBUG_PRINT(("Inserting sound into map\n"));
    if (map->sound_count % SOUND_REALLOC_STEP == 0) {
        DEBUG_PRINT(("At sound count %d, need to realloc another block\n", map->sound_count));
        int new_chunks = (map->sound_count / SOUND_REALLOC_STEP) + 1;
        map->sounds = realloc(map->sounds, new_chunks * 100 * sizeof(struct sound*));
        DEBUG_PRINT(("Successfully realloc'd space\n"));
    }
    map->sounds[map->sound_count++] = sound;
    DEBUG_PRINT(("Successfully inserted sound\n"));
}

void 
clear_sounds(struct map *map)
{
    for (int i = 0; i < map->sound_count; i++) {
        DEBUG_PRINT(("Clearing sound, number: %d\n", i));
        struct sound *sound = map->sounds[i];
        sound->cell->sound = NULL;
        // If we reference the type from the 'all_sounds' array, don't free it - else free internals
        if (!(sound->type >= all_sound_types && sound->type < (all_sound_types + all_sound_types_count))) {
            DEBUG_PRINT(("Sound was not referenced from all_sound_types, deinit\n"));
            deinit_sound(sound);
        }
        free(sound);
    }
    DEBUG_PRINT(("Freeing sound array\n"));
    free(map->sounds);
    map->sound_count = 0;
    // Must be null for future realloc
    map->sounds = NULL;
}

void 
insert_entity(struct map *map, struct ent entity, struct vector2i position)
{
    struct cell *cel = get_cell(map, &position);
	struct ent *entity_copy = init_entity(malloc(sizeof (struct ent)), entity.name, entity.desc, entity.icon, entity.weight, entity.size);
    entity_copy->position = position;
	entity_copy->map = map;
	
	entity_copy->prev = NULL;
	entity_copy->next = cel->inventory;
	cel->inventory->prev = entity_copy;
	
	cel->inventory = entity_copy;
	cel->inventory_size += 1;
}

void
map_remove_entity(struct ent *entity)
{
	struct cell *cell = get_cell(entity->map, &entity->position);
	if (entity->prev == NULL) {
		cell->inventory = entity->next;
	} else {
		entity->prev->next = entity->next;
	}
	entity->position = (struct vector2i){ 0, 0 };
	entity->map = NULL;
	entity->prev = NULL;
	entity->next = NULL;
}

struct ctr*
insert_creature(struct map *map, struct ctr *creature, struct scrpt *script, struct vector2i position)
{
	struct cell *cell = get_cell(map, &position);
	struct ctr *new_creature = &map->creatures[map->creature_count];
	new_creature->idx = map->creature_count;
    new_creature->position = position;
    new_creature->health = creature->health;
    new_creature->tp = creature->tp;
    new_creature->inventory_size = creature->inventory_size;
	new_creature->hearing = creature->hearing;
    new_creature->name = malloc(strlen(creature->name) + 1);
    strcpy(new_creature->name, creature->name);
    new_creature->desc = malloc(strlen(creature->desc) + 1);
    strcpy(new_creature->desc, creature->desc);
    new_creature->inventory = NULL;
    new_creature->map = map;
    new_creature->script = script;
    new_creature->speech = new_memory();
    new_creature->memory = new_memory();
	cell->creature = new_creature;
	map->creature_count++;
	return new_creature;
}

void
map_consume_speech(struct ctr *creature)
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
        return;
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
    struct sound *sound = sound_creature(sound_type, creature);
	if (sound) {
		insert_sound(creature->map, sound);
		DEBUG_PRINT(("Generated speech: %s\n", sound->type->text));
	}
}


char
cell_to_char(struct cell *cel)
{
	if (cel == NULL || cel->ground < 0)
		return CELL_SPACE_ICON;

	struct grnd ground = all_grounds[(int) cel->ground];
	
    char rtn = ground.icon;

	
	struct ent *entity = cel->inventory;
	size_t invsz = cel->inventory_size;
	if (invsz > 0) {
		rtn = entity->icon;
	}
	
    struct ctr *creature = cel->creature;
    if (creature) {
        rtn = creature->name[0];
    }
    
    return rtn;
}

void
destroy_map(struct map* map)
{
	free(map->creatures);
	free(map->cells);
	free(map);
}

bool
cell_blocks_sight(struct cell *c)
{
	return false;
}
