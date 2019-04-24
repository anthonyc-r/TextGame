#include "creature.h"
#include "map.h"

void
creature_walk(struct ctr *creature, enum dir_tp direction)
{
	struct pnt dest_pnt = {creature->position->x, creature->position->y};
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
	current_cell = get_cell(creature->position);
	dest_cell = get_cell(&dest_pnt);
	if (!dest_cell || !current_cell) {
		return;
	}
	
	//Void cells are unpassable by walking.
	if (dest_cell->ground == SPACE || dest_cell->ground == -1) 
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
		creature->position->x = dest_pnt.x;
		creature->position->y = dest_pnt.y;
	}
}