#ifndef _SCRIPT_H
#define _SCRIPT_H
#include <stdio.h>
#include <stdbool.h>

#define ACTION_ATTACK "attack"

struct mem;
struct ent;
struct ctr;

enum script_status {
	TURN_END, TURN_CONTINUE, NO_MATCH
};

struct script_line {
	char *op;
	int argc;
	char **argv;
};

struct scrpt_state_location {
	int line_number;
	char *identifier;
};

struct action {
	int line_number;
	char *identifier;
	char *key;
};

struct reaction {
	int line_number;
	char *identifier;
	char *key;
};

struct scrpt {
	int current_line;
	int current_state;
	int state_count;
	struct scrpt_state_location *state_locations;
	int line_count;
	struct script_line *lines;
	struct mem *memory;
	
	int act_count;
	struct action *actions;
	int react_count;
	struct reaction *reactions;
};

struct scrpt *script_init(char *fname);
struct script_line script_extract_line(char *string);
void script_reset(struct scrpt *script);
enum script_status script_perform_line(struct scrpt *script, struct mem *stack, struct script_line *line);

void script_perform_entity(struct scrpt *script, struct ent *entity);
void script_perform_creature(struct scrpt *script, struct ctr *creature);
enum script_status script_perform_line_creature(struct scrpt *script, struct ctr *creature, struct script_line *line);
enum script_status script_perform_line_entity(struct scrpt *script, struct ent *entity, struct script_line *line);

struct reaction *script_get_reaction(struct scrpt *script, char *identifier, char *key);
struct action *script_get_action(struct scrpt *script, char *identifier);
bool script_entity_act_on_creature(struct ent *entity, struct ctr *creature, char *action);

bool begins_with(char *s1, char *s2);
char *get_operator(char *s1);
char *get_operand(char *s1);
void script_describe(struct scrpt *script);
#endif
