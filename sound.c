#include <stdlib.h>

#include "sound.h"
#include "map.h"
#include "creature.h"
#include "entity.h"

struct sound *
sound_creature(struct sound_t *t, struct ctr *creature)
{
	struct sound *sound = malloc(sizeof(struct sound));
	sound->type = t;
	sound->source_t = SOUND_SOURCE_CREATURE;
	sound->source.creature = creature;
    struct cell *cell = get_cell(creature->map, &creature->position);
    sound->cell = cell;
    cell->sound = sound;
	return sound;
}

struct sound *
sound_entity(struct sound_t *t, struct ent *entity)
{
	struct sound *sound = malloc(sizeof(struct sound));
	sound->type = t;
	sound->source_t = SOUND_SOURCE_ENTITY;
	sound->source.entity = entity;
    struct cell *cell = get_cell(entity->map, &entity->position);
    sound->cell = cell;
    cell->sound = sound;
	return sound;
}

void 
deinit_sound(struct sound *sound)
{
    free(sound->type);
}
