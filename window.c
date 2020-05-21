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
print_main(void)
{
	printf("%s", main_window->data);
}
