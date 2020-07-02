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

struct _GameEditorApp
{
	GtkApplication parent;
};


G_DEFINE_TYPE(GameEditorApp, game_editor_app, GTK_TYPE_APPLICATION);


static void
game_editor_app_activate(GApplication *app)
{
	GameEditorMainWindow *win;

	g_debug("app activated");	
	win = game_editor_main_window_new(GAME_EDITOR_APP(app));
	gtk_window_present(GTK_WINDOW(win));
}

static void
game_editor_app_init(GameEditorApp *app)
{
}

static void
game_editor_app_class_init(GameEditorAppClass *class)
{
	G_APPLICATION_CLASS(class)->activate = game_editor_app_activate;
}

GameEditorApp *game_editor_app_new()
{
	return g_object_new(GAME_EDITOR_APP_TYPE, NULL);
}
