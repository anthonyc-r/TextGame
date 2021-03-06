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


#ifndef TEXTUI_H
#define TEXTUI_H

#include "entity.h"

#define TUI_MAX_WINDOWS 100
#define TUI_MAX_LINES 100
#define TUI_LINE_LENGTH 100

enum tui_mode {
	TUI_MODE_WALK,
	TUI_MODE_TALK,
	TUI_MODE_PICKUP,
	TUI_MODE_EQUIP,
	TUI_MODE_ATTACK,
	TUI_MODE_USE
};

struct tui_info {
	enum tui_mode mode;
	char talkbuf[100];
	int talklen;
	
	struct ent **piclist;
	int picount;
	
	char logbuf[TUI_MAX_LINES][TUI_LINE_LENGTH];
	int logbuf_idx;
	
	struct ctr **ctrlist;
	int ctrcount;
	
	struct ent *uitem;
	
} tui_info;

#endif
