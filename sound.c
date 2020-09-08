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
