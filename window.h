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

struct wnw *new_window(int x, int y, int width, int height);
void clear_window(struct wnw *window, char clear_char);
void destroy_window(struct wnw *window);
void draw_to_main(struct wnw *window);
void window_put_text(struct wnw *window, char *text, enum window_style style);
void print_main(void);
#endif
