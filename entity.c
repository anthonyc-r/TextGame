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


#include "entity.h"
#include <stdlib.h>
#include <string.h>
int
entity_blocking(struct ent *entity)
{
	switch (entity->size) {
		case SIZE_HUGE:
			return 1;
        case SIZE_LARGE:
            return 1;
		default:
			return 0;
	}
}

struct ent*
init_entity(struct ent *entity, char *name, char *desc, char icon, int weight, enum size_type size)
{
	entity->idx = -1;
	entity->name = name;
	entity->desc = desc;
	entity->icon = icon;
	entity->weight = weight;
	entity->size = size;
	entity->script = NULL;
	entity->next = NULL;
	entity->prev = NULL;
	return entity;
}

struct ent
new_entity(char *name, char *desc, char icon, int weight, enum size_type size)
{
	struct ent entity;
	init_entity(&entity, name, desc, icon, weight, size);
	return entity;
}

enum size_type 
entity_size(char *string)
{
	int len = strlen(string);
	if (strncmp(string, "SIZE_HUGE", len) == 0) {
		return SIZE_HUGE;
	} else if (strncmp(string, "SIZE_LARGE", len) == 0) {
		return SIZE_LARGE;
	} else if (strncmp(string, "SIZE_SMALL", len) == 0) {
		return SIZE_SMALL;
	} else if (strncmp(string, "SIZE_TINY", len) == 0) {
		return SIZE_TINY;
	} else {
		return SIZE_TINY;
	}
}

bool entity_use_creature(struct ent *entity, struct ctr *creature) {
	script_entity_act_on_creature(entity, creature, NULL);
}
