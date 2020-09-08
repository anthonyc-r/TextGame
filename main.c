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


//define DEBUG
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "debug.h"
#include "rouge.h"
#include "map.h"
#include "vector.h"
#include "creature.h"
#include "window.h"
#include "systemspecific.h"
#include "maploader.h"
#include "script.h"
#include "gameloader.h"
#include "memory.h"
#include "textui.h"
#include "entity.h"
#include "narrator.h"

#define CH_ESCAPE '\x1b'

struct opt_windows {
	int num;
	struct wnw *windows;
} windows;

void draw(struct map* map);

void
init_data(char *data_path)
{
	printf("init game data\n");
	windows.num = 0;
	//init game data
	FILE *data_file = fopen(data_path, "r");
	if (data_file == NULL) {
		printf("Failed to open data file. Does it exist?\n");
		return;
	}
	load_game(data_file);
	active_map = load_map(data_file);
	fclose(data_file);
	DEBUG_PRINT(("Load game, load map, done.\n"));
	struct vector2i plr_position;
	plr_position.x = 10;
	plr_position.y = 10;
	struct ctr plr = new_creature("Zoltan", "Powerful mage", 100, 100, 2);
	plr.hearing = 5;
	player = insert_creature(active_map, &plr, NULL, plr_position);
	DEBUG_PRINT(("done inserting test objects\n"));
	//init display data
	main_window = new_window(0, 0, 60, 20);
	clear_window(main_window, '*');
	
	map_window = new_window(0, 0, 40, 15);
	clear_window(map_window, ' ');
	
	info_window = new_window(25, 0, 25, 2);
	clear_window(info_window, ' ');
    
	chat_window = new_window(0, 15, 60, 5);
	clear_window(chat_window, '+');
	
	inv_window = new_window(40, 2, 20, 14);
	clear_window(inv_window, ' ');
	
	talk_window = new_window(10, 5, 40, 3);
	clear_window(talk_window, '+');
	
	pickup_window = new_window(20, 4, 20, 12);
	clear_window(pickup_window, '+');
	
	alert_window = new_window(5, 7, 30, 3);
	clear_window(alert_window, ' ');
	
	target_window = new_window(20, 4, 20, 12);
	clear_window(target_window, ' ');
	
	tui_info = (struct tui_info) { TUI_MODE_WALK, "", 0, 0, 0 , 0, 0};
	printf("game data init\n");
}

void 
render_info(struct wnw *window)
{
	size_t max_len = window->width * window->height; //Allow for null.
	size_t actual_len = max_len + 1;
	char *info_txt = malloc(actual_len);
	clear_window(info_window, ' ');
	snprintf(info_txt, actual_len, "Name: %s Health: %d  TP: %d X: %d Y: %d", player->name, player->health, player->tp, player->position.x, player->position.y);
	actual_len = strlen(info_txt);

	memcpy(window->data, info_txt, actual_len);
	
	free(info_txt);
}

void 
render_chat(struct wnw *window)
{
    struct sound **soundlist = creature_listen(player);
    struct sound *sound;
    char *message = NULL;
    for (int i = 0; (sound = soundlist[i]); i++) {
        DEBUG_PRINT(("Found sounds!!\n"));
        strcpy(tui_info.logbuf[tui_info.logbuf_idx], sound->type->text);
    	tui_info.logbuf_idx = (tui_info.logbuf_idx + 1) % TUI_MAX_LINES;
    }
    while ((message = next_narration(&main_narrator)) != NULL) {
    	strcpy(tui_info.logbuf[tui_info.logbuf_idx], message);
    	tui_info.logbuf_idx = (tui_info.logbuf_idx + 1) % TUI_MAX_LINES;
    }
    clear_window(window, ' ');
    window_fill_border(window, '-');
	for (int i = 1; i < window->height; i++) {
		int idx = (tui_info.logbuf_idx - window->height) + i + 1;
		if (idx < 0)
			idx = TUI_MAX_LINES + idx;

		window_put_line(window, tui_info.logbuf[idx], i, WINDOW_STYLE_NONE);
	}
}

void 
render_talk(struct wnw *window)
{
	clear_window(window, ' ');
	tui_info.talkbuf[tui_info.talklen] = '\0';
	window_fill_border(window, '0');
	window_put_text(window, tui_info.talkbuf, WINDOW_STYLE_BORDERED);
	memcpy(window->data + 3, "say", 3);
}

void
render_pickup(struct wnw *window)
{
	clear_window(window, ' ');
	window_fill_border(window, '0');
	memcpy(window->data + 3, "pick up", 7);
	for (int i = 0; i < tui_info.picount; i++) {
		char *name = tui_info.piclist[i]->name;
		char entry[strlen(name) + 3];
		// todo: - make work for more items...
		sprintf(entry, "%c) %s", 'a' + i, name);
		window_put_line(window, entry, i + 1, WINDOW_STYLE_BORDERED);
	}
}

void
render_inv(struct wnw *window)
{
	clear_window(window, ' ');
	window_fill_border(window, '-');
	memcpy(window->data + 3, "inventory", 9);
	for (int i = 0; i < player->inventory_size; i++) {
		struct ent *item = player->inventory[i];
		if (tui_info.mode == TUI_MODE_EQUIP || tui_info.mode == TUI_MODE_USE) {
			char *name = item->name;
			char labeled[strlen(name) + 3];
			sprintf(labeled, "%c) %s", 'a' + (char)i, name);
			window_put_line(window, labeled, i + 1, WINDOW_STYLE_BORDERED);
		} else if ((int)creature_equip_location(player, item) > -1) {
			char *name = item->name;
			char labaled[strlen(name) + 1];
			sprintf(labaled, "*%s", name);
			window_put_line(window, labaled, i + 1, WINDOW_STYLE_BORDERED);
		} else {
			window_put_line(window, item->name, i + 1, WINDOW_STYLE_BORDERED);
		}
	}
}

void
render_target(struct wnw *window)
{
	clear_window(window, ' ');
	window_fill_border(window, '0');
	memcpy(window->data + 3, "target?", 7);
	struct ctr **around = creature_look(player);
	struct ctr *current;
	int i = 0;
	for (i = 0; (current = around[i]) != NULL; i++) {
		char labeled[strlen(current->name) + 3];
		sprintf(labeled, "%c) %s", 'a' + i, current->name);
		window_put_line(window, labeled, i + 1, WINDOW_STYLE_BORDERED);	
	}
	int sz = i * sizeof (struct ctr*);
	if (tui_info.ctrlist != NULL) {
		free(tui_info.ctrlist);
		tui_info.ctrlist = NULL;
	}
	tui_info.ctrcount = i;
	tui_info.ctrlist = around;
}

void
draw(struct map *map)
{
	DEBUG_PRINT(("Draw\n"));
    struct vector2i view_from = {player->position.x - map_window->width / 2, player->position.y - map_window->height / 2};
    struct vector2i view_to = {view_from.x + map_window->width, view_from.y + map_window->height};
	//clear
	clear_window(main_window, ' ');
	systemspecific_clear();
	//redraw
    render_map(map, map_window, &view_from, &view_to);
	draw_to_main(map_window);
	render_info(info_window);
	draw_to_main(info_window);
    render_chat(chat_window);
    draw_to_main(chat_window);
	render_inv(inv_window);
	draw_to_main(inv_window);
	
	switch (tui_info.mode) {
		case TUI_MODE_TALK:
			render_talk(talk_window);
			draw_to_main(talk_window);
			break;
		case TUI_MODE_PICKUP:
			render_pickup(pickup_window);
			draw_to_main(pickup_window);
			break;
		case TUI_MODE_EQUIP:
			draw_to_main(alert_window);
			break;
		case TUI_MODE_USE:
			if (tui_info.uitem != NULL) {
				render_target(target_window);
				draw_to_main(target_window);
			} else {
				draw_to_main(alert_window);
			}
			break;
	}
	
	for (int i = 0; i < windows.num; ++i) {
		struct wnw tmp_wnw = windows.windows[i];
		draw_to_main(&tmp_wnw);
	}
	print_main();
}

int getkey() {
    return systemspecific_getch();
}

void
window_test()
{
	struct wnw *test = malloc(sizeof(struct wnw));
	test->width = 30;
	test->height = 4;
	test->size = test->width * test->height;
	test->data = malloc(test->size + test->height + 1);
	test->top_left = malloc(sizeof(struct vector2i));
	(test->top_left)->x = 25;
	(test->top_left)->y = 13;
	clear_window(test, '+');
	char *text = "You do 10 damage.";
	memcpy((test->data + 38), text, strlen(text));
	window_put_text(test, text, WINDOW_STYLE_BORDERED_CENTER);
	windows.num = 1;
	windows.windows = test;
}

void
player_walk(enum dir_t dir)
{
	if (!creature_walk(player, dir)) {
		// blocked
		struct ctr *creature = creature_look_direction(player, dir);
		if (creature) {
		 	if (creature_attack(player, creature)) {
		 		DEBUG_PRINT(("triggered attack!\n"));
		 	} else {
		 		narrate(&main_narrator, "Nothing happens.");
		 	}	
		}
	}
}
void
perform_action_walk(char c)
{
	switch (c) {
		case 'w':
			player_walk(NORTH);
			break;
		case 'a':
			player_walk(WEST);
			break;
		case 's':
			player_walk(SOUTH);
			break;
		case 'd':
			player_walk(EAST);
			break;
		case 't':
			tui_info.mode = TUI_MODE_TALK;
			break;
		case 'e':
			clear_window(alert_window, ' ');
			window_put_line(alert_window, "Equip what?", 1, WINDOW_STYLE_BORDERED_CENTER);
			tui_info.mode = TUI_MODE_EQUIP;
			break;
		case 'p':
			tui_info.mode = TUI_MODE_PICKUP;
			tui_info.picount = creature_search_items(player, NULL);
			tui_info.piclist = malloc(tui_info.picount * sizeof (struct ent*));
			creature_search_items(player, tui_info.piclist);
			break;
		case 'u':
			clear_window(alert_window, ' ');
			window_put_line(alert_window, "Use what?", 1, WINDOW_STYLE_BORDERED_CENTER);
			tui_info.mode = TUI_MODE_USE;
			tui_info.uitem = NULL;
			break;
		case 'm':
			window_test();
			return;
		default:
			break;
	}
}

void
perform_action_talk(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ') {
		tui_info.talkbuf[tui_info.talklen++] = c;
	} else if (c == '\x7f') { // delete
		if (tui_info.talklen > 0)
			tui_info.talklen--;
	} else if (c == '\n') {
		tui_info.talkbuf[tui_info.talklen] = '\0';
		creature_say_str(player, tui_info.talkbuf);
		map_consume_speech(player);
		tui_info.talklen = 0;
		tui_info.mode = TUI_MODE_WALK;
	} else if (c == CH_ESCAPE) {
		tui_info.talklen = 0;
		tui_info.mode = TUI_MODE_WALK;
	}
	return;	
}

void
perform_action_pickup(char c)
{
	if (c == CH_ESCAPE) {
		tui_info.mode = TUI_MODE_WALK;
		tui_info.picount = 0;
		if (tui_info.piclist != NULL) {
			free(tui_info.piclist);
			tui_info.piclist = NULL;
		}
	} else {
		int idx = c - 'a';
		if (idx < 0 || idx >= tui_info.picount) {
			return;
		}
		struct ent *item = tui_info.piclist[idx];
		if (creature_take_entity(player, item)) {
			map_remove_entity(item);
			free(tui_info.piclist);
		} else {
			narrate(&main_narrator, "inventory full!");
		}
		tui_info.mode = TUI_MODE_WALK;
	}
}

void
perform_action_equip(char c)
{
	if (c == CH_ESCAPE) {
		tui_info.mode = TUI_MODE_WALK;
	} else {
		int idx = c - 'a';
		clear_window(alert_window, ' ');
		if (idx < 0 || c > 'z' || idx >= player->inventory_size) {
			window_put_line(alert_window, "Invalid selection!", 1, WINDOW_STYLE_BORDERED_CENTER);
		} else {
			struct ent *item = player->inventory[idx];
			if (creature_equip(player, item, EQUIP_LOCATION_WEAPON)) {
				tui_info.mode = TUI_MODE_WALK;
			} else {
				window_put_line(alert_window, "You can't equip that!", 1, WINDOW_STYLE_BORDERED_CENTER);
			}
		}
	}
}


void
perform_action_use(char c)
{
	if (c == CH_ESCAPE) {
		tui_info.mode = TUI_MODE_WALK;
		if (tui_info.ctrlist != NULL) {
			free(tui_info.ctrlist);
			tui_info.ctrlist = NULL;
		}
		tui_info.ctrcount = 0;
	} else if (tui_info.uitem == NULL) {
		// is picking an item
		int idx = c - 'a';
		if (idx >= player->inventory_size || idx < 0) {
			clear_window(alert_window, ' ');
			window_put_line(alert_window, "Invalid selection!", 1, WINDOW_STYLE_BORDERED_CENTER);
		} else {
			tui_info.uitem = player->inventory[idx];
		}
	} else {
		// is picking a target
		int idx = c - 'a';
		if (idx >= tui_info.ctrcount || idx < 0) {
			clear_window(alert_window, ' ');
			window_put_line(alert_window, "Invalid selection!", 1, WINDOW_STYLE_BORDERED_CENTER);
		} else {
			entity_use_creature(tui_info.uitem, tui_info.ctrlist[idx]);
			free(tui_info.ctrlist);
			tui_info.ctrlist = NULL;
			tui_info.mode = TUI_MODE_WALK;
		}
	}
}

void
perform_action(char c)
{
	switch (tui_info.mode) {
		case TUI_MODE_WALK:
			perform_action_walk(c);
			break;
		case TUI_MODE_TALK:
			perform_action_talk(c);
			break;
		case TUI_MODE_PICKUP:
			perform_action_pickup(c);
			break;
		case TUI_MODE_EQUIP:
			perform_action_equip(c);
			break;
		case TUI_MODE_USE:
			perform_action_use(c);
			break;
	}	
}

void
logic(struct map *map)
{
	DEBUG_PRINT(("Logic\n"));
	// clean up
	clear_sounds(active_map);
	// get input
	char c;
	while ((c = getkey())) {
		printf("%c", c);
		perform_action(c);
		break;
	}
	
	for (int i = 0; i < map->creature_count; i++) {
		struct ctr *creature = &map->creatures[i];
		DEBUG_PRINT(("Performing script for creature %s\n", creature->name));
		if (creature->script)
			script_perform_creature(creature->script, creature);
	}
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s game.dat\n", argv[0]);
		return 1;
	}
	systemspecific_clear();
	init_data(argv[1]);
	printf("Game start - name: %s\n", player->name);
	systemspecific_clear();
 	while (1) {
		draw(active_map);
		logic(active_map);
	}
	printf("Game end\n");
	return 0;
}
