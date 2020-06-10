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


#ifndef _WINDOW_H
#define _WINDOW_H

enum window_style {
	WINDOW_STYLE_BORDERED_CENTER,
	WINDOW_STYLE_BORDERED,
	WINDOW_STYLE_NONE
};

struct wnw {
	int width;
	int height;
	int size;
	struct vector2i *top_left;
	char *data;
};
struct wnw *main_window;
struct wnw *map_window;
struct wnw *info_window;
struct wnw *chat_window;
struct wnw *talk_window;
struct wnw *pickup_window;
struct wnw *inv_window;

struct wnw *new_window(int x, int y, int width, int height);
void clear_window(struct wnw *window, char clear_char);
void destroy_window(struct wnw *window);
void draw_to_main(struct wnw *window);
void window_put_text(struct wnw *window, char *text, enum window_style style);
void window_put_line(struct wnw *window, char *text, int line, enum window_style style);
void window_fill_border(struct wnw *window, char b);
void print_main(void);
#endif
