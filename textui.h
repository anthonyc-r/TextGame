#ifndef TEXTUI_H
#define TEXTUI_H

#include "entity.h"

#define TUI_MAX_WINDOWS 100

enum tui_mode {
	TUI_MODE_WALK,
	TUI_MODE_TALK,
	TUI_MODE_PICKUP
};

struct tui_info {
	enum tui_mode mode;
	char talkbuf[100];
	int talklen;
	
	struct ent *piclist;
	int picount;
} tui_info;

#endif
