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

#define ADD_TREE(P, STACK, ID, MODEL) \
	P = GTK_TREE_VIEW(gtk_tree_view_new_with_model(MODEL)); \
	gtk_widget_show(GTK_WIDGET(P)); \
	gtk_stack_add_named(STACK, GTK_WIDGET(P), ID); \
	gtk_tree_view_append_column(P, gtk_tree_view_column_new_with_attributes("Types", gtk_cell_renderer_text_new(), "text", 0, NULL));


struct _EditorMainWindow
{
	GtkApplicationWindow parent;
	
	GtkComboBoxText *res_combo;
	GtkStack *res_stack;
	GtkTreeView *grounds_tree;
	GtkTreeView *ent_tree;
	GtkTreeView *creat_tree;
};

G_DEFINE_TYPE(EditorMainWindow, editor_main_window, GTK_TYPE_APPLICATION_WINDOW);


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

	gtk_combo_box_set_active(GTK_COMBO_BOX(window->res_combo), 0);
}

static void
editor_main_window_class_init(EditorMainWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/main_window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorMainWindow, res_combo);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorMainWindow, res_stack);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), res_combo_changed);
}

EditorMainWindow *
editor_main_window_new(EditorApp *app)
{
	return g_object_new(EDITOR_MAIN_WINDOW_TYPE, "application", app, "title", "Game Editor", NULL);
}

