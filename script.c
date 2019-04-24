//define DEBUG

#include "script.h"
#include "gameloader.h"
#include "utility.h"
#include "memory.h"
#include "map.h"
#include "creature.h"
#include "debug.h"
#include "entity.h"
#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STATE_IDENTIFIER "-state"
#define MEMORY_SIZE 100
#define OPERATOR_MAX_LENGTH 100
#define OPERAND_MAX_LENGTH 100

struct scrpt*
script_init(char *fname)
{
	struct scrpt *script = malloc(sizeof (struct scrpt));
	FILE *fp = fopen(fname, "r");
	fpos_t fstart;
	int numlines = 0;
	int numstates = 0;
	char *line;
	
	script->memory = new_memory();
	script->current_line = 1;
	script->current_state = 0;
	fgetpos(fp, &fstart);
	for (numlines = 0; (line = read_line(fp)); numlines++) {
		if (strncmp(get_operator(line), STATE_IDENTIFIER, strlen(STATE_IDENTIFIER)) == 0)
			numstates++;
		DEBUG_PRINT(("Read line %d\n", numlines));
	}
	DEBUG_PRINT(("Read script, found %d lines, and %d states.\n", numlines, numstates));
	script->line_count = numlines;
	script->state_count = numstates;
	script->lines = malloc(numlines * sizeof (struct script_line));
	DEBUG_PRINT(("malloc line storage\n"));
	script->state_locations = malloc(numstates * sizeof(struct scrpt_state_location));
	DEBUG_PRINT(("malloc location storage\n"));
	fsetpos(fp, &fstart);
	numstates = 0;
	DEBUG_PRINT(("Copying script to memory...\n"));
	for (int i = 0; (line = read_line(fp)); i++) {
		script->lines[i] = script_extract_line(line);
		DEBUG_PRINT(("Stored line %d\n", i));
		if (strncmp(script->lines[i].op, STATE_IDENTIFIER, strlen(STATE_IDENTIFIER)) == 0) {
			struct scrpt_state_location loc;
			loc.identifier = script->lines[i].argv[0];
			loc.line_number = i;
			script->state_locations[numstates] = loc;
			DEBUG_PRINT(("Stored state location %d\n", numstates));
			numstates++;
		}
	}
	fclose(fp);
	return script;
}

void
script_reset(struct scrpt *script)
{
	int state_num = script->current_state;
	int line_number = script->state_locations[state_num].line_number;
	script->current_line = line_number + 1;
}

struct script_line
script_extract_line(char *string)
{
	struct script_line line;
	line.argv = malloc(sizeof(char *));
	line.argc = 1;
	line.op = get_operator(string);
	line.argv[0] = get_operand(string);
	return line;
}

enum script_status
script_parse_line(struct scrpt *script, struct mem *stack, struct script_line *line)
{
	int cstate = script->current_state;
	char *op = line->op;
	int opsz = strlen(op);
	if (opsz == 0)
		return TURN_END; // No op, end turn.
	char *arg = line->argv[0];

	DEBUG_PRINT(("Read line %s\n", line->op));
	#ifdef DEBUG
	if (arg == NULL)
		DEBUG_PRINT(("No arg\n"));
	else
		DEBUG_PRINT(("arg: %s\n", arg));
	#endif
	if (strcmp(op, "-state") == 0) {
		script->current_line = script->state_locations[cstate].line_number;
		DEBUG_PRINT(("Invoked -state\n"));
		return TURN_END;
	} else if (strcmp(op, "set_state") == 0) {
		//TODO: OPTIMIZE
		for (int i = 0; i < script->state_count; i++) {
			struct scrpt_state_location *state = &script->state_locations[i];
			if (strcmp(state->identifier, arg) == 0) {
				script->current_state = i;
				DEBUG_PRINT(("Invoked set_state %s\n", arg));
				return TURN_END;
			}
		}
		DEBUG_PRINT(("State identifier not found: %s\n", arg));
		exit(-1);
	} else if (strcmp(op, "\n") == 0) {
		return TURN_END;
	} else if (strcmp(op, "if") == 0) {
		DEBUG_PRINT(("Invoked if\n"));
		if (!memory_eq_string(script->memory, arg))
			script->current_line++;
		return TURN_CONTINUE;
	} else if (strcmp(op, "remember") == 0) {
		if (arg == NULL) {
			memory_push_copy(&stack, script->memory);
		} else {
			struct mem *mem = new_memory();
			memory_set_string(mem, arg);
			memory_push_copy(&stack, mem);
		}
		DEBUG_PRINT(("Invoked remember\n"));
		return TURN_CONTINUE;
	} else if (strcmp(op, "recall") == 0) {
		struct mem *mem = memory_pop(&stack);
		if (mem) {
			deinit_memory(script->memory);
            		//free(script->memory);
			script->memory = mem;
		}
		DEBUG_PRINT(("Invoked recall\n"));
		return TURN_CONTINUE;
	} else if (strcmp(op, "forget") == 0) {
		DEBUG_PRINT(("forget invoked\n"));
		deinit_memory(memory_pop(&stack));
		return TURN_CONTINUE;
	} else if (strcmp(op, "forget_all") == 0) {
		DEBUG_PRINT(("forget_all invoked\n"));
		do {
			deinit_memory(memory_pop(&stack));
		} while (stack->next);
		return TURN_CONTINUE;
	} else {
		return NO_MATCH;
	}
}

void 
script_perform_entity(struct scrpt *script, struct ent *entity)
{
	enum script_status status;
	struct script_line *line;
	do {
		line = &script->lines[script->current_line];
		status = script_parse_line(script, entity->memory, line);
		if (status == NO_MATCH)
			status = script_perform_line_entity(script, entity, line);
		script->current_line++;
	} while (status == TURN_CONTINUE);
	if (status == NO_MATCH) {
		DEBUG_PRINT(("Invalid operator in script\n"));
		exit(-1);
	}
	script_reset(script);
	DEBUG_PRINT(("======== turn over: %s =========\n", entity->name));
}


void 
script_perform_creature(struct scrpt *script, struct ctr *creature)
{
	enum script_status status;
	struct script_line *line;
	do {
		line = &script->lines[script->current_line];
		status = script_parse_line(script, creature->memory, line);
		if (status == NO_MATCH)
			status = script_perform_line_creature(script, creature, line);
		script->current_line++;
	} while (status == TURN_CONTINUE);
	if (status == NO_MATCH) {
		DEBUG_PRINT(("Invalid operator in script\n"));
		exit(-1);
	}
	script_reset(script);
    DEBUG_PRINT(("Generating speech for turn...\n"));
    struct sound *sound = creature_consume_speech(creature);
    if (sound) {
        insert_sound(creature->map, sound);
        DEBUG_PRINT(("Generated speech: %s\n", sound->type->text));
    }
	DEBUG_PRINT(("======== turn over: %s =========\n", creature->name));
}

enum script_status
script_perform_line_entity(struct scrpt *script, struct ent *entity, struct script_line *line) {
	return TURN_END;
}

enum script_status
script_perform_line_creature(struct scrpt *script, struct ctr *creature, struct script_line *line)
{
	char *op = line->op;
	int opsz = strlen(op);
	if (opsz == 0)
		return TURN_END; // No op, end turn.
	char *arg = line->argv[0];
	DEBUG_PRINT(("Read line %s\n", line->op));
	#ifdef DEBUG
	if (arg == NULL)
		DEBUG_PRINT(("No arg\n"));
	else
		DEBUG_PRINT(("arg: %s\n", arg));
	#endif
	if (strcmp(op, "say") == 0) {
        struct mem *memory = new_memory();
		if (arg == NULL) {
            memory_set_string(memory, "not implemented yet");
            memory_push(&creature->speech, memory);
			DEBUG_PRINT(("Invoked say noop\n"));
		} else {
			memory_set_string(memory, arg);
            memory_push(&creature->speech, memory);
			DEBUG_PRINT(("Invoked say (%s)\n", arg));
		}
		return TURN_CONTINUE;
	} else if (strcmp(op, "look_creature") == 0) {
		DEBUG_PRINT(("Creature is looking for others\n"));
		struct ctr **creatures = creature_look(creature);
		DEBUG_PRINT(("Looked for creatures\n"));
		while (*creatures != NULL) {
			DEBUG_PRINT(("not null\n"));
			struct mem *memory = new_memory();
			memory_set_creature(memory, *creatures);
			memory_push(&creature->memory, memory);
			++creatures;
		}
		DEBUG_PRINT(("look_creature invoked\n"));
		return TURN_CONTINUE;
	} else if (strcmp(op, "move_toward") == 0) {
		struct ctr *memcreature = memory_active_creature(script->memory, creature->map);
		if (memcreature) {
			bool near_target = creature_walk_toward(creature, memcreature);
			if (near_target) {
				memory_set_string(script->memory, "DONE");
			}
		}
		return TURN_CONTINUE;
	} else {
		return NO_MATCH;
	}
}

bool
begins_with(char *identifier, char *s)
{
	int slen = strlen(s);
	int identifierlen = strlen(identifier);
	if (slen < identifierlen)
		return false;
	return memcmp(identifier, s, identifierlen) == 0;
}

char*
get_operator(char *s)
{
	int len = strlen(s);
	char *op = malloc(OPERATOR_MAX_LENGTH);
	int i;
	for (i = 0; i < len && i < OPERATOR_MAX_LENGTH && s[i] != ' '; i++);
	strncpy(op, s, i);
	op[i] = 0;
	return op;
}

char*
get_operand(char *s)
{
	int len = strlen(s);
	char *op = malloc(OPERATOR_MAX_LENGTH);
	int i;
	for (i = 0; i < len && i < OPERATOR_MAX_LENGTH && s[i] != ' '; i++);
	if (i == len)
		return NULL;
	strncpy(op, &s[i + 1], len - i + 1); // Skip space at i
	return op;
}

void 
script_describe(struct scrpt *script)
{
	printf("SCRIPT LINES:\n");
	for (int i = 0; i < script->line_count; i++) {
		printf("LINE %3d: %s %s\n", i, script->lines[i].op, script->lines[i].argv[0]);
	}
	printf("STATE COUNT: %d\n", script->state_count);
	printf("STATE LOCATIONS:\n");
	for (int i = 0; i < script->state_count; i++) {
		struct scrpt_state_location loc = script->state_locations[i];
		printf("	STATE:\n");
		printf("	STATE NAME: %s\n", loc.identifier);
		printf("	STATE LINE: %d\n", loc.line_number);
	}
	printf("END SCRIPT\n");
}
