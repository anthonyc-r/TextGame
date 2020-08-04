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
	EditorMainWindow *main_window;
	GtkListStore *ground_types;
	GtkListStore *entity_types;
	GtkListStore *creature_types;
	struct map *map;
};


G_DEFINE_TYPE(EditorApp, editor_app, GTK_TYPE_APPLICATION);

static void
editor_app_activate(GApplication *app)
{
	EditorMainWindow *window;

	g_debug("app activated");	
	window = editor_main_window_new(EDITOR_APP(app));
	EDITOR_APP(app)->main_window = window;
	gtk_window_present(GTK_WINDOW(window));
}

static void
quit_activated(GSimpleAction *action, GVariant *param, gpointer app)
{
	g_application_quit(G_APPLICATION(app));
}

static void 
ground_created(void *userdata)
{
	struct ground *ground = userdata;
	g_debug("ground created, name: %s, char: %c", ground->name, ground->icon);
	editor_app_add_ground(current_app, ground);
}

static void
create_ground_activated(GSimpleAction *action, GVariant *param, gpointer p)
{
	g_debug("tapped create ground");
	struct ground *ground = new_ground("", '-');
	EditorCreateResourceWindow *window = editor_create_resource_window_new(current_app, ground_created, (void*)ground,
		RESOURCE_FIELD_NAME, "name", &ground->name,
		RESOURCE_FIELD_CHAR, "icon", &ground->icon, 
		0);
	gtk_window_present(GTK_WINDOW(window));
}

static int
count_model(GtkTreeModel *model)
{
	GtkTreeIter iter;
	int count = 0;
	if (gtk_tree_model_get_iter_first(model, &iter)) {
		do {
			count++;
		} while (gtk_tree_model_iter_next(model, &iter));
	}
	return count;
}

static void
save_game_activated(GSimpleAction *action, GVariant *param, gpointer p)
{
	int nground, ncreature, nentity, count;
	nground = count_model(GTK_TREE_MODEL(current_app->ground_types));
	ncreature = count_model(GTK_TREE_MODEL(current_app->creature_types));
	nentity = count_model(GTK_TREE_MODEL(current_app->entity_types));
	g_debug("counted list lengths %d, %d, %d", nground, ncreature, nentity);
	struct ground **grounds = malloc((nground + 1) * sizeof (struct ground*));
	struct creature **creatures = malloc((ncreature + 1) * sizeof (struct creature*));
	struct entity **entities = malloc((nentity + 1) * sizeof (struct entity*));
	GtkTreeModel *model = GTK_TREE_MODEL(current_app->ground_types);
	GtkTreeIter iter;
	void *value;
	count = 0;
	if (gtk_tree_model_get_iter_first(model, &iter)) {
		do {
			gtk_tree_model_get(model, &iter, 1, &value, -1);
			grounds[count++] = (struct ground*)value;
		} while (gtk_tree_model_iter_next(model, &iter));
	}
	grounds[count] = NULL;
	model = GTK_TREE_MODEL(current_app->creature_types);
	count = 0;
	if (gtk_tree_model_get_iter_first(model, &iter)) {
		do {
			gtk_tree_model_get(model, &iter, 1,  &value, -1);
			creatures[count++] = (struct creature*)value;
		} while (gtk_tree_model_iter_next(model, &iter));
	}
	creatures[count] = NULL;
	model = GTK_TREE_MODEL(current_app->entity_types);
	count = 0;
	if (gtk_tree_model_get_iter_first(model, &iter)) {
		do {
			gtk_tree_model_get(model, &iter, 1, &value, -1);
			entities[count++] = (struct entity*)value;
		} while (gtk_tree_model_iter_next(model, &iter));
	}
	entities[count] = NULL;
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Save Game Data",
		GTK_WINDOW(current_app->main_window), 
		GTK_FILE_CHOOSER_ACTION_SAVE,
		"Cancel", GTK_RESPONSE_CANCEL,
		"Save", GTK_RESPONSE_ACCEPT, NULL);
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy(dialog);
		return;
	}
	char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	gtk_widget_destroy(dialog);
	save_game_data(filename, entities, grounds, creatures);
	free(filename);
	free(grounds);
	free(entities);
	free(creatures);
	g_debug("saved");
}

static void
load_game_activated(GSimpleAction *action, GVariant *param, gpointer p)
{
	g_debug("tapped load game");
	struct ground **grounds;
	struct entity **entities;
	struct creature **creatures;
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Load Game Data",
		GTK_WINDOW(current_app->main_window), 
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel", GTK_RESPONSE_CANCEL,
		"Load", GTK_RESPONSE_ACCEPT, NULL);
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy(dialog);
		return;
	}
	char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	gtk_widget_destroy(dialog);
	load_game_data(filename, &entities, &grounds, &creatures);
	g_debug("game data loaded");
	free(filename);
	while (*grounds != NULL) {
		editor_app_add_ground(current_app, *grounds);
		grounds++;
	}
	while (*entities != NULL) {
		editor_app_add_entity(current_app, *entities);
		entities++;
	}
	while (*creatures != NULL) {
		editor_app_add_creature(current_app, *creatures);
		creatures++;
	}
}

static GActionEntry editor_app_entries[] = 
{
	{"quit", quit_activated, NULL, NULL, NULL },
	{"load_game", load_game_activated, NULL, NULL, NULL},
	{"save_game", save_game_activated, NULL, NULL, NULL},
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
	app->map = new_map(10, 10);
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
void 
editor_app_add_creature(EditorApp *app, struct creature *creature)
{
	GtkTreeIter iter;
	gtk_list_store_append(app->creature_types, &iter);
	gtk_list_store_set(app->creature_types, &iter, 0, creature->name, 1, creature, -1);
}
void 
editor_app_add_entity(EditorApp *app, struct entity *entity)
{
	GtkTreeIter iter;
	gtk_list_store_append(app->entity_types, &iter);
	gtk_list_store_set(app->entity_types, &iter, 0, entity->name, 1, entity, -1);
}

struct map *
editor_app_get_map(EditorApp *app)
{
	return app->map;
}
