#include "ground.h"

struct grnd 
new_ground(char *name, char icon)
{
	struct grnd ground;
	ground.name = name;
	ground.icon = icon;
	return ground;
}