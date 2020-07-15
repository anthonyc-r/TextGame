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

#ifndef _ENTITY_H
#define _ENTITY_H

#define MAX_NAME 10

struct entity {
	char name[MAX_NAME];
	int weight;
};
struct creature {
	char name[MAX_NAME];
	int health;
};
struct ground {
	char name[MAX_NAME];
	char icon;
};

struct entity *new_entity(char *name, int weight);
struct ground *new_ground(char *name, char icon);
struct creature *new_creature(char *name, int health);
#endif
