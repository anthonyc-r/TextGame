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
#include <string.h>

#include "app.h"
#include "main_window.h"
#include "game_data.h"
#include "create_resource_window.h"

struct _EditorApp
{
	GtkApplication parent;
	GtkListStore *ground_types;
	GtkListStore *entity_types;
	GtkListStore *creature_types;
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

static void
create_ground_activated(GSimpleAction *action, GVariant *param, gpointer p)
{
	g_debug("tapped create ground");
	EditorCreateResourceWindow *window = editor_create_resource_window_new(current_app, NULL);
	gtk_window_present(GTK_WINDOW(window));
}


static GActionEntry editor_app_entries[] = 
{
	{"quit", quit_activated, NULL, NULL, NULL },
	{"create_ground", create_ground_activated, NULL, NULL, NULL}
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
	app->ground_types = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	app->creature_types = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	app->entity_types = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	
	// some example dat
	GtkTreeIter iter;
	gtk_list_store_append(app->ground_types, &iter);
	gtk_list_store_set(app->ground_types, &iter, 0, "mud", 1, new_ground("mud", '~'), -1);
	gtk_list_store_append(app->ground_types, &iter);
	gtk_list_store_set(app->ground_types, &iter, 0, "stone", 1, new_ground("stone", '.'), -1);
	gtk_list_store_append(app->ground_types, &iter);
	gtk_list_store_set(app->ground_types, &iter, 0, "marble", 1, new_ground("marble", '*'), -1);
	gtk_list_store_append(app->ground_types, &iter);
	gtk_list_store_set(app->ground_types, &iter, 0, "grass", 1, new_ground("grass", '`'), -1);
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

GtkTreeModel *
editor_app_get_ground_tree_model(EditorApp *app)
{
	return GTK_TREE_MODEL(app->ground_types);
}

GtkTreeModel *
editor_app_get_entity_tree_model(EditorApp *app)
{
	return GTK_TREE_MODEL(app->entity_types);
}

GtkTreeModel *
editor_app_get_creature_tree_model(EditorApp *app)
{
	return GTK_TREE_MODEL(app->creature_types);
}

void 
editor_app_add_ground(EditorApp *app, struct ground *ground)
{
	GtkTreeIter iter;
	gtk_list_store_append(app->ground_types, &iter);
	gtk_list_store_set(app->ground_types, &iter, 0, ground->name, 1, ground, -1);
}
