#include <stdio.h>
#include <stdlib.h>

#include "window.h"
#include "vector.h"

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
print_main(void)
{
	printf("%s", main_window->data);
}
