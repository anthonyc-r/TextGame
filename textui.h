#ifndef TEXTUI_H
#define TEXTUI_H

enum tui_mode {
	TUI_MODE_WALK,
	TUI_MODE_TALK,
	TUI_MODE_PICKUP
};

struct tui_info {
	enum tui_mode mode;
	char talkbuf[100];
	int talklen;
} tui_info;

#endif
