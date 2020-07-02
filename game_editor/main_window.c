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

#include "app.h"
#include "main_window.h"

struct _GameEditorMainWindow
{
	GtkApplicationWindow parent;
};

G_DEFINE_TYPE(GameEditorMainWindow, game_editor_main_window, GTK_TYPE_APPLICATION_WINDOW);

static void
game_editor_main_window_init(GameEditorMainWindow *window)
{
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
}

static void
game_editor_main_window_class_init(GameEditorMainWindowClass *class)
{
}

GameEditorMainWindow *
game_editor_main_window_new(GameEditorApp *app)
{
	return g_object_new(GAME_EDITOR_MAIN_WINDOW_TYPE, "application", app, "title", "Game Editor", NULL);
}

void
game_editor_main_window_open(GameEditorMainWindow *win, GFile *file)
{
}
