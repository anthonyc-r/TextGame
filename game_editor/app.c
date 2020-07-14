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
#include "game_data.h"

struct _EditorApp
{
	GtkApplication parent;
	GArray *ground_types;
	GArray *entity_types;
	GArray *creature_types;
};


G_DEFINE_TYPE(EditorApp, editor_app, GTK_TYPE_APPLICATION);


static void
editor_app_activate(GApplication *app)
{
	EditorMainWindow *window;

	g_debug("app activated");	
	window = editor_main_window_new(EDITOR_APP(app));

	
	gtk_window_present(GTK_WINDOW(window));
}

static void
quit_activated(GSimpleAction *action, GVariant *param, gpointer app)
{
	g_application_quit(G_APPLICATION(app));
}


static GActionEntry editor_app_entries[] = 
{
	{"quit", quit_activated, NULL, NULL, NULL }
};

static void
editor_app_startup(GApplication *app)
{
	G_APPLICATION_CLASS(editor_app_parent_class)->startup(app);
	g_action_map_add_action_entries(G_ACTION_MAP(app), editor_app_entries, G_N_ELEMENTS(editor_app_entries), app);
}

static void
editor_app_init(EditorApp *app)
{
	app->ground_types = g_array_new(FALSE, FALSE, sizeof (struct ground));
	app->creature_types = g_array_new(FALSE, FALSE, sizeof (struct creature));
	app->entity_types = g_array_new(FALSE, FALSE, sizeof (struct entity));
	struct ground test = { "mud", 'm' };
	g_array_append_val(app->ground_types, test);
}

static void
editor_app_class_init(EditorAppClass *class)
{
	G_APPLICATION_CLASS(class)->activate = editor_app_activate;
	G_APPLICATION_CLASS(class)->startup = editor_app_startup;
}

EditorApp *
editor_app_new()
{
	return g_object_new(EDITOR_APP_TYPE, NULL);
}
