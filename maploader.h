#ifndef _MAPLOADER_H
#define _MAPLOADER_H

struct map;

struct map *load_map(char *filename);
int ascii_dec(char *ascii);
int read_metadata(FILE *fp, int *width, int *height);
#endif
