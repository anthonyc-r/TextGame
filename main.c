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

static bool draw_talk = false;
static char talkbuf[100];
static int talklen = 0;


struct opt_windows {
	int num;
	struct wnw *windows;
} windows;

void draw(struct map* map);

void
init_data(void)
{
	printf("init game data\n");
	windows.num = 0;
	//init game data
	load_game("data//game.dat");
    active_map = load_map("data//map.dat");
    DEBUG_PRINT(("Load game, load map, done.\n"));
    struct vector2i plr_position;
    plr_position.x = 14;
    plr_position.y = 13;
	struct ctr plr = new_creature("Zoltan", "Powerful mage", 100, 100, 0);
	plr.hearing = 5;
	player = insert_creature(active_map, &plr, NULL, plr_position);
	DEBUG_PRINT(("done inserting test objects\n"));
	//init display data
    main_window = new_window(0, 0, 60, 20);
	clear_window(main_window, '|');
	
    map_window = new_window(0, 0, 40, 15);
	clear_window(map_window, ' ');
	
    info_window = new_window(25, 0, 25, 2);
	clear_window(info_window, ' ');
    
    chat_window = new_window(1, 16, 58, 3);
    clear_window(chat_window, '+');
	
	talk_window = new_window(10, 5, 40, 3);
	clear_window(talk_window, '+');
	
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
        message = sound->type->text;
    }
    clear_window(window, '+');
    if (message) {
		window_put_text(window, message, WINDOW_STYLE_BORDERED);
    } else {
		window_put_text(window, "no sounds", WINDOW_STYLE_BORDERED);
	}
}

void 
render_talk(struct wnw *window)
{
	clear_window(window, '+');
	talkbuf[talklen] = '\0';
	window_put_text(window, talkbuf, WINDOW_STYLE_BORDERED);
	memcpy(window->data + 3, "say", 3);
}

void
draw(struct map *map)
{
	DEBUG_PRINT(("Draw\n"));
    struct vector2i view_from = {player->position.x - 5, player->position.y - 5};
    struct vector2i view_to = {view_from.x + map_window->width, view_from.y + map_window->height};
	//clear
	clear_window(main_window, '|');
	systemspecific_clear();
	//redraw
    render_map(map, map_window, &view_from, &view_to);
	draw_to_main(map_window);
	render_info(info_window);
	draw_to_main(info_window);
    render_chat(chat_window);
    draw_to_main(chat_window);
	if (draw_talk) {
		render_talk(talk_window);
		draw_to_main(talk_window);
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
perform_action(char c)
{
	if (draw_talk) {
		if ((c >= 'a' && c <= 'z') || c == ' ') {
			talkbuf[talklen++] = c;
		} else if (c == '\x7f') { // delete
			if (talklen > 0)
				talklen--;
		} else if (c == '\n') {
			talkbuf[talklen] = '\0';
			creature_say_str(player, talkbuf);
			map_consume_speech(player);
			talklen = 0;
			draw_talk = false;
		} else if (c == '\x1b') { // escape
			talklen = 0;
			draw_talk = false;
		}
		return;
	}
	
	enum dir_t dir;
	switch (c) {
		case 'w':
			creature_walk(player, NORTH);
			break;
		case 'a':
			creature_walk(player, WEST);
			break;
		case 's':
			creature_walk(player, SOUTH);
			break;
		case 'd':
			creature_walk(player, EAST);
			break;
		case 't':
			draw_talk = true;
			break;
		case 'm':
			window_test();
			return;
		default:
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
	//printf("loading script\n");
	//struct scrpt *script = script_init("data_devel//test.script");
    ////script_describe(script);
	//exit(1);
	systemspecific_clear();
    init_data();
    printf("Game start - name: %s\n", player->name);
	systemspecific_clear();
	
	/*
	while (true) {
		script_perform(script, &player);
		char *s = player->speech->string;
		if (s != NULL)
			printf("Speech string: %s\n", s);
		printf("======END TURN======\n");
		char c = getkey();
		if (c == 'q')
			break;
	}
	exit(0);
	*/
 	while (1) {
		draw(active_map);
		logic(active_map);
	}
    printf("Game end\n");
    return 0;
}
