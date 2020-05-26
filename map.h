#ifndef _MAP_H
#define _MAP_H

//top left == 0,0
#include<stdlib.h>
#include<stdbool.h>

struct scrpt;
struct sound;
struct wnw;
struct map;
struct cell;
struct vector2i;
struct ent;
struct sound;
enum dir_t;

static const char CELL_SPACE_ICON = '.';


struct map {
	int width;
	int height;
	int size;
	int creature_count;
	struct ctr *creatures;
	int sound_count;
	struct sound **sounds;
	struct cell *cells;
} *active_map;

struct cell {
    char ground;
    
    struct cell *north;
    struct cell *east;
    struct cell *south;
    struct cell *west;
    
	int inventory_size;
    struct ent *inventory;
    struct ctr *creature;
    struct sound *sound;
};

struct map *init_map(int width, int height);
void destroy_map(struct map *map);
void render_map(struct map *map, struct wnw *window, struct vector2i *from, struct vector2i *to);
void insert_entity(struct map *map, struct ent entity, struct vector2i position);
void insert_sound(struct map *map, struct sound *sound);
void clear_sounds(struct map *map);
struct ctr *insert_creature(struct map *map, struct ctr *creature, struct scrpt *script, struct vector2i position);
void map_consume_speech(struct ctr *creature);
void destroy_inventory(struct ent *inventory);
char cell_to_char(struct cell *cel);
struct cell *dir_to_cell(struct cell *cel, enum dir_t direction);
struct cell *get_cell(struct map *map, struct vector2i *point);
bool cell_blocks_sight(struct cell *c);
bool cell_blocks_movement(struct cell *c);
#endif
