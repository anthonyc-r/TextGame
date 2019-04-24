#ifndef _GROUND_H
#define _GROUND_H

#include<stdbool.h>

struct grnd {
	char *name;
	char icon;
} *all_grounds;

struct grnd new_ground(char *name, char icon);
#endif
