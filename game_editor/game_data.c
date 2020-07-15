/*
Copyright 2020 Anthony Cohn-Richardby

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

#include <string.h>
#include <stdlib.h>

#include "game_data.h"

static inline void 
cpyname(char *dst, char *src)
{
	strncpy(dst, src, MAX_NAME);
	dst[MAX_NAME - 1] = '\0';
}

struct entity *
new_entity(char *name, int weight) 
{
	struct entity *entity = malloc(sizeof(struct entity));
	cpyname(entity->name, name);
	entity->weight = weight;
	return entity;
}

struct ground *
new_ground(char *name, char icon)
{
	struct ground *ground = malloc(sizeof(struct ground));
	cpyname(ground->name, name);
	ground->icon = icon;
	return ground;
}


struct creature *
new_creature(char *name, int health)
{
	struct creature *creature = malloc(sizeof(struct creature));
	cpyname(creature->name, name);
	creature->health = health;
	return creature;
}
