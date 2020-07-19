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
#include "create_resource_window.h"
#include "game_data.h"

struct _EditorCreateResourceWindow
{
	GtkApplicationWindow parent;
	GtkBox *vbox;
	void *value_holders;
	enum resource_field_type *field_types;
	char **field_titles;
	GtkEntry *field_entries;
	int field_count;
};

G_DEFINE_TYPE(EditorCreateResourceWindow, editor_create_resource_window, GTK_TYPE_APPLICATION_WINDOW);

static void
editor_create_resource_window_add_field(GtkBox *box) 
{
	
}

static void
editor_create_resource_window_finalize(GObject *object) 
{
	EditorCreateResourceWindow *window = EDITOR_CREATE_RESOURCE_WINDOW(object);
	for (int i = 0; i < window->field_count; i++) {
		free(window->field_titles[i]);
	}
	free(window->field_entries);
	free(window->field_titles);
	free(window->field_types);
	G_OBJECT_CLASS(editor_create_resource_window_parent_class)->finalize(object);
}

static void
editor_create_resource_window_init(EditorCreateResourceWindow *window)
{
	gtk_widget_init_template(GTK_WIDGET(window));
	editor_app_add_ground(current_app, new_ground("water", '~'));
}

static void
editor_create_resource_window_class_init(EditorCreateResourceWindowClass *class)
{
	G_OBJECT_CLASS(class)->finalize = editor_create_resource_window_finalize;

	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/create_resource_window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorCreateResourceWindow, vbox);
}

// Call like editor_create_resource_window_new(app, RESOURCE_FIELD_STRING, "name", RESOURCE_FIELD_INT, "hp", 0);
EditorCreateResourceWindow *
editor_create_resource_window_new(EditorApp *app, ...)
{
	g_debug("editor_create_resource_window_new...");
	int argcount = 0;
	EditorCreateResourceWindow *window = g_object_new(EDITOR_CREATE_RESOURCE_WINDOW_TYPE, "application", app, "title", "New Resource", NULL);
	g_debug("config...");
	va_list argp;
	va_start(argp, app);
	while (va_arg(argp, int) != (int)_RESOURCE_FIELD_END) {
		va_arg(argp, void*);
		argcount += 1;
	}
	window->value_holders = malloc(argcount * sizeof (void *));
	window->field_types = malloc(argcount * sizeof (enum resource_field_type));
	window->field_titles = malloc(argcount * sizeof (char **));
	window->field_entries = malloc(argcount * sizeof (GtkEntry *));
	window->field_count = argcount;
	
	va_start(argp, app);
	for (int i = 0; i < argcount; i++) {
		window->field_types[i] = va_arg(argp, enum resource_field_type);
		char *title = va_arg(argp, char*);
		window->field_titles[i] = malloc(strlen(title) + 1);
		strcpy(window->field_titles[i], title);
	}
	
	// Setup ui fields
	for (int i = 0; i < window->field_count; i++) {
		g_debug("adding field %d, %s", i, window->field_titles[i]);
		gtk_box_pack_start(window->vbox, gtk_label_new(window->field_titles[i]), FALSE, FALSE, 10);
	}
	gtk_widget_show_all(GTK_WIDGET(window->vbox));
	
	return window;
}
