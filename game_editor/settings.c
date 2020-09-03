/*
Copyright 2020 Anthony Cohn-Richardby

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
#include <gtk/gtk.h>

#include "settings.h"

static char setting_value[MAX_SETTING_LEN];

static GFileIOStream *
open_settings_stream()
{
	const char *path = g_build_filename(g_get_user_config_dir(), 
		"game_editor.conf", NULL);
	g_debug("config file: %s", path);
	GFile *file = g_file_new_for_path(path);
	GError *error = NULL;
	GFileIOStream *stream = g_file_create_readwrite(file, G_FILE_CREATE_NONE, 
		NULL, &error);
	if (error != NULL) {
		if (error->code == G_IO_ERROR_EXISTS) {
			g_error_free(error);
			error = NULL;
			stream = g_file_open_readwrite(file, NULL, &error);
			if (error != NULL) {
				g_error("Failed to open settings file. (%s)", 
					error->message);
				g_error_free(error);
				return NULL;
			}
		} else {
			g_error("Failed to open the settings file. (%s)", error->message);
			g_error_free(error);
			return NULL;
		}
	}
	return stream;
}

static char *
readline(GInputStream *istream, goffset *line_start)
{
	g_debug("read line");
	char *line, c;
	int line_len = 0;
	*line_start = g_seekable_tell(G_SEEKABLE(istream));
	while (g_input_stream_read(istream, &c, 1, NULL, NULL) == 1 && c != '\n')
		line_len++;
	if (line_len < 1)
		return NULL;
	line = malloc(line_len + 1);
	g_seekable_seek(G_SEEKABLE(istream), *line_start, G_SEEK_SET, NULL, NULL);
	g_input_stream_read_all(istream, line, line_len, NULL, NULL, NULL);
	// Read past the newline
	g_seekable_seek(G_SEEKABLE(istream), 1, G_SEEK_CUR, NULL, NULL);
	line[line_len] = '\0';
	return line;
}

static int
scan_for_setting(const char *key, GFileIOStream *stream, goffset *line_start, char **line)
{
	g_seekable_seek(G_SEEKABLE(stream), 0, G_SEEK_SET, NULL, NULL);
	GInputStream *istream;
	g_object_get(stream, "input-stream", &istream, NULL);
	int res = 0;
	while ((*line = readline(istream, line_start)) != NULL) {
		g_debug("comparing '%s' with '%s'", *line, key);
		if (strncmp(*line, key, strlen(key)) == 0) {
			res = 1;
			break;
		}
		free(*line);
	}
	g_object_unref(istream);
	return res;
}

static void
append_line(GFileIOStream *iostream, const char *line)
{	
	g_debug("Will append line");
	GOutputStream *ostream;
	g_object_get(iostream, "output-stream", &ostream, NULL);
	g_seekable_seek(G_SEEKABLE(ostream), 0, G_SEEK_END, NULL, NULL);
	g_output_stream_write_all(ostream, line, strlen(line), NULL, NULL, NULL);
	g_output_stream_write(ostream, "\n", 1, NULL, NULL);
	g_object_unref(ostream);
}

static void
overwrite_line(GFileIOStream *iostream, const char *existing_line, int existing_len, const char *new_line, int new_len, goffset loc)
{
	g_debug("Will overwrite line current len %d, new len %d", existing_len, new_len);
	GOutputStream *ostream;
	GInputStream *istream;
	gsize fend, shiftlen, nread;
	char *lbuff;

	g_seekable_seek(G_SEEKABLE(iostream), 0, G_SEEK_END, NULL, NULL);
	fend = g_seekable_tell(G_SEEKABLE(iostream));
	shiftlen = fend - (loc + existing_len);
	
	g_object_get(iostream, "output-stream", &ostream, NULL);
	g_object_get(iostream, "input-stream", &istream, NULL);
	// Need to shift the file around to make room
	if (new_len != existing_len) {
		g_debug("Shifting last %ld of file", shiftlen);
		lbuff = malloc(shiftlen);
		g_seekable_seek(G_SEEKABLE(ostream), loc + existing_len, G_SEEK_SET, NULL, NULL);
		g_input_stream_read_all(istream, lbuff, shiftlen, &nread, NULL, NULL);
		g_seekable_seek(G_SEEKABLE(ostream), loc + new_len, G_SEEK_SET, NULL, NULL);
		g_output_stream_write_all(ostream, lbuff, nread, NULL, NULL, NULL);
		free(lbuff);
		if (new_len < existing_len) {
			fend = g_seekable_tell(G_SEEKABLE(ostream));
			g_seekable_truncate(G_SEEKABLE(ostream), fend, NULL, NULL);
		}
	}
	g_seekable_seek(G_SEEKABLE(iostream), loc, G_SEEK_SET, NULL, NULL);
	g_output_stream_write_all(ostream, new_line, new_len, NULL, NULL, NULL);
	g_object_unref(ostream);
	g_object_unref(istream);
}

static void 
settings_set(const char *name, const char *value)
{
	g_debug("setting value for %s", name);
	GFileIOStream *iostream = open_settings_stream();
	char *existing_line, *new_line;
	goffset line_start;
	int existing_len, new_len;
	
	new_len = strlen(name) + strlen(value) + 2;
	new_line = malloc(new_len);
	snprintf(new_line, new_len, "%s\t%s", name, value);
	new_line[new_len - 1] = '\0';
	
	if (scan_for_setting(name, iostream, &line_start, &existing_line)) {
		existing_len = strlen(existing_line);
		overwrite_line(iostream, existing_line, existing_len, new_line, 
			new_len - 1, line_start);
		free(existing_line);
	} else {
		append_line(iostream, new_line);
	}
	g_io_stream_close(G_IO_STREAM(iostream), NULL, NULL);
}

static const char*
settings_get(const char *name, const char *def)
{
	GFileIOStream *iostream = open_settings_stream();
	char *line, *res, *tok;
	goffset line_start;
	if (scan_for_setting(name, iostream, &line_start, &line)) {
		strtok(line, "\t");
		tok = strtok(NULL, "\t");
		strncpy(setting_value, tok, MAX_SETTING_LEN);
		free(line);
	} else {
		strncpy(setting_value, def, MAX_SETTING_LEN);
	}
	setting_value[MAX_SETTING_LEN - 1] = '\0';
	g_io_stream_close(G_IO_STREAM(iostream), NULL, NULL);
	return setting_value;
}

const char *
settings_get_terminal_emulator()
{
	return settings_get("terminal_emulator", "xterm");
}

void
settings_set_terminal_emulator(const char *new_value)
{
	settings_set("terminal_emulator", new_value);
}
