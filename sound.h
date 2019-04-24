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
