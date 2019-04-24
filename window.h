#ifndef _WINDOW_H
#define _WINDOW_H

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

struct wnw *new_window(int x, int y, int width, int height);
void clear_window(struct wnw *window, char clear_char);
void destroy_window(struct wnw *window);
void draw_to_main(struct wnw *window);
void print_main(void);
#endif
