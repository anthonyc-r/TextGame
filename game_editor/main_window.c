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
#include "headerbar.h"
#include "game_data.h"
#include "properties_view.h"

#define ADD_TREE(P, STACK, ID, MODEL) \
	P = GTK_TREE_VIEW(gtk_tree_view_new_with_model(MODEL)); \
	gtk_widget_show(GTK_WIDGET(P)); \
	gtk_stack_add_named(STACK, GTK_WIDGET(P), ID); \
	gtk_tree_view_append_column(P, gtk_tree_view_column_new_with_attributes("Types", gtk_cell_renderer_text_new(), "text", 0, NULL));	

struct _EditorMainWindow
{
	GtkApplicationWindow parent;
	
	EditorPropertiesView *prop_view;
	
	GtkGrid *map_grid;
	GtkComboBoxText *res_combo;
	GtkStack *res_stack;
	GtkTreeView *grounds_tree;
	GtkTreeView *ent_tree;
	GtkTreeView *creat_tree;
	GtkBox *prop_container;
};

G_DEFINE_TYPE(EditorMainWindow, editor_main_window, GTK_TYPE_APPLICATION_WINDOW);

struct point {
	int x, y;
};


static void
set_active_item(EditorMainWindow *window, enum active_item_type active_item_type, void *item)
{
	g_debug("updated active item");
	editor_properties_view_set_item(window->prop_view, active_item_type, item);
}

static void
grid_button_clicked(GtkButton *button, gpointer user_data)
{
	void **data = (void**)user_data;
	// see setup_map_grid, g_signal_connect_data.
	struct cell *cell = (struct cell*)data[0];
	EditorMainWindow *window = EDITOR_MAIN_WINDOW(data[1]);
	set_active_item(window, ACTIVE_ITEM_CELL, (void*)cell);
}

static void
setup_map_grid(EditorMainWindow *window, GtkGrid *map_grid, struct map *map)
{
	g_debug("set up map grid!");
	for (int i = 0; i < map->width; i++) {
		gtk_grid_insert_column(map_grid, 0);
	}
	for (int i = 0; i < map->height; i++) {
		gtk_grid_insert_row(map_grid, 0);
	}
	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			struct cell *cell = map_get_cell(map, x, y);
			void **userdata = malloc(2 * sizeof (void*));
			userdata[0] = cell;
			userdata[1] = window;
			GtkWidget *btn = gtk_button_new_with_label("X");
			gtk_grid_attach(map_grid, btn, x, y, 1, 1);
			g_signal_connect_data(G_OBJECT(btn), "clicked", G_CALLBACK(grid_button_clicked), userdata, (GClosureNotify)free, 0);
			gtk_widget_show(btn);
		}
	}
}

static void
tree_view_row_activated(GtkTreeView *tree_view, gpointer user_data)
{
	EditorMainWindow *window = EDITOR_MAIN_WINDOW(user_data);
	GtkTreePath *path;
	gtk_tree_view_get_cursor(tree_view, &path, NULL);
	if (path == NULL) {
		g_debug("null path, row activated");
		return;
	}
	GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
	GtkTreeIter iter;
	gtk_tree_model_get_iter(model, &iter, path);
	void *item;
	gtk_tree_model_get(model, &iter, 1, &item, -1);
	gtk_tree_path_free(path);

	if (tree_view == window->grounds_tree) {
		set_active_item(window, ACTIVE_ITEM_GROUND, item);
	} else if (tree_view == window->ent_tree) {
		set_active_item(window, ACTIVE_ITEM_ENTITY, item);
	} else if (tree_view == window->creat_tree) {
		set_active_item(window, ACTIVE_ITEM_CREATURE, item);
	}
}

static void
res_combo_changed(GtkComboBox *widget, EditorMainWindow *win) 
{
	GtkWidget *btn;
	EditorApp *app;
	int active;
	
	app = EDITOR_APP(gtk_window_get_application(GTK_WINDOW(win)));
	active = gtk_combo_box_get_active(widget);
	g_debug("changed combo box %d!", active);
	switch (active) {
		case 0:
			gtk_stack_set_visible_child_name(win->res_stack, GROUND_RES_ID);
			break;
		case 1:
			gtk_stack_set_visible_child_name(win->res_stack, CREATURE_RES_ID);
			break;
		case 2:
			gtk_stack_set_visible_child_name(win->res_stack, ENTITY_RES_ID);
			break;
	}
}

static void
editor_main_window_init(EditorMainWindow *window)
{
	gtk_widget_init_template(GTK_WIDGET(window));
	gtk_window_set_titlebar(GTK_WINDOW(window), GTK_WIDGET(editor_headerbar_new()));

	// setup res combo box
	gtk_combo_box_text_append(window->res_combo, GROUND_RES_ID, "Ground");
	gtk_combo_box_text_append(window->res_combo, CREATURE_RES_ID, "Creature");
	gtk_combo_box_text_append(window->res_combo, ENTITY_RES_ID, "Entity");
	// Set up res stack
	ADD_TREE(window->grounds_tree, window->res_stack, GROUND_RES_ID, editor_app_get_ground_tree_model(current_app));
	ADD_TREE(window->creat_tree, window->res_stack, CREATURE_RES_ID, editor_app_get_creature_tree_model(current_app));
	ADD_TREE(window->ent_tree, window->res_stack, ENTITY_RES_ID, editor_app_get_entity_tree_model(current_app));
	g_signal_connect(G_OBJECT(window->grounds_tree), "cursor-changed", G_CALLBACK(tree_view_row_activated), window);
	g_signal_connect(G_OBJECT(window->creat_tree), "cursor-changed", G_CALLBACK(tree_view_row_activated), window);
	g_signal_connect(G_OBJECT(window->ent_tree), "cursor-changed", G_CALLBACK(tree_view_row_activated), window);
	gtk_combo_box_set_active(GTK_COMBO_BOX(window->res_combo), 0);
	// map
	setup_map_grid(window, window->map_grid, editor_app_get_map(current_app));
	// insert properties view
	window->prop_view = editor_properties_view_new();
	gtk_box_pack_start(window->prop_container, GTK_WIDGET(window->prop_view), TRUE, TRUE, 0);
	gtk_widget_show(GTK_WIDGET(window->prop_view));
}

static void
editor_main_window_class_init(EditorMainWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/main_window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorMainWindow, res_combo);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorMainWindow, res_stack);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorMainWindow, map_grid);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorMainWindow, prop_container);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), res_combo_changed);
}

EditorMainWindow *
editor_main_window_new(EditorApp *app)
{
	return g_object_new(EDITOR_MAIN_WINDOW_TYPE, "application", app, "title", "Game Editor", NULL);
}

