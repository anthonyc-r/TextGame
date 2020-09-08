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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "window.h"
#include "vector.h"
#include "utility.h"

struct wnw *
new_window(int x, int y, int width, int height)
{
	struct wnw *window = malloc(sizeof(struct wnw));
	window->width = width;
	window->height = height;
	window->size = width * height;
	window->data = malloc(window->size + height + 1);
	window->top_left = malloc(sizeof(struct vector2i));
	(window->top_left)->x = x;
	(window->top_left)->y = y;
    return window;
}

void
clear_window(struct wnw *window, char clear_char)
{   
	int index = 0;
	for (int i = 0; i < window->height; ++i) {
		for (int j = 0; j < window->width; ++j) {
			window->data[index] = clear_char;
			++index;
		}
		window->data[index] = '\n';
		++index;
	}
	window->data[index] = '\0';
}

void
destroy_window(struct wnw *window)
{
	free(window->data);
	free(window->top_left);
	free(window);
}

void
draw_to_main(struct wnw *window)
{
	struct vector2i *top_left = window->top_left;
	int space_after = main_window->width - (top_left->x + window->width) + 1;
	
	size_t main_index = top_left->x + top_left->y * (main_window->width + 1);
	size_t sub_index = 0;
	for (int i = 0; i < window->height; ++i) {
		for (int j = 0; j < window->width; ++j) {
			main_window->data[main_index] = window->data[sub_index];
			++main_index;
			++sub_index;
		}
		main_index += space_after + top_left->x;
		//newline char
		sub_index += 1;
	}
}

void
window_put_text(struct wnw *window, char *text, enum window_style style)
{
	int line_width, line_count, len;
	
	len = strlen(text);
	switch (style) {
		case WINDOW_STYLE_NONE:
			if (window->width * window->height < len)
				break;
			memcpy(window->data, text, len);
			break;
		case WINDOW_STYLE_BORDERED_CENTER:
			if ((window->width - 2) * (window->height - 2) < len) 
				break;
			line_width = window->width - 2;
			line_count = len / line_width;
			for (int i = 0; i <= line_count; i++) {
				int data_offset = (i + 1) * window->width + 2;
				int text_offset = i * line_width;
				int count = MIN(len - text_offset, line_width);
				int chars_before = (line_width - count) / 2;
				data_offset += chars_before;
				memcpy(window->data + data_offset, text + text_offset, count);  
			}
			break;
		case WINDOW_STYLE_BORDERED:
			if ((window->width - 2) * (window->height - 2) < len) 
				break;
			line_width = window->width - 2;
			line_count = len / line_width;
			for (int i = 0; i <= line_count; i++) {
				int data_offset = (i + 1) * window->width + 2;
				int text_offset = i * line_width;
				int count = MIN(len - text_offset, line_width);
				memcpy(window->data + data_offset, text + text_offset, count);  
			}
			break;
		default:
			break;
	}
}

void
window_put_line(struct wnw *window, char *text, int line, enum window_style style)
{
	int maxh = window->height;
	int maxw = window->width;
	int indent = 0;
	if (style == WINDOW_STYLE_BORDERED || style == WINDOW_STYLE_BORDERED_CENTER) {
		maxh -= 1;
		maxw -= 1;
		indent = 1;
	}
	if (line >= maxh)
		return;
	int len = MIN(maxw, strlen(text));
	int offset = line * (window->width + 1) + indent;
	if (style == WINDOW_STYLE_BORDERED_CENTER) {
		offset += (maxw - len) / 2;
	}
	memcpy(window->data + offset, text, len);
}

void
window_fill_border(struct wnw *window, char b) {
	// + newlines...
	int maxidx = ((window->width + 1) * window->height) - 1;
	for (int i = 0; i < window->width; i++) {
		window->data[i] = b;
		window->data[maxidx - i] = b;
	}
	for (int i = 0; i < maxidx; i += window->width + 1) {
		window->data[i] = b;
		window->data[i + window->width - 1] = b;
	}
}

void
print_main(void)
{
	printf("%s", main_window->data);
}
