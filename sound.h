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


#ifndef _SOUND_H
#define _SOUND_H

struct ent;
struct ctr;
struct cell;

enum sound_source_t {
	SOUND_SOURCE_ENTITY, SOUND_SOURCE_CREATURE
};

union sound_source {
	struct ctr *creature;
	struct ent *entity;
};

struct sound_t {
	char *text;
	char *id;
} *all_sound_types;
int all_sound_types_count;

struct sound {
	struct sound_t *type;
	union sound_source source;
    struct cell *cell;
	enum sound_source_t source_t;
};

struct sound *sound_creature(struct sound_t *t, struct ctr *creature);
struct sound *sound_entity(struct sound_t *t, struct ent *entity);
struct sound sound_creature_say(struct ctr *creature, char *speech);
void deinit_sound(struct sound *sound);

#endif
