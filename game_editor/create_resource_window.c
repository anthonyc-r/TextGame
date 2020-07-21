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
	void **value_holders;
	enum resource_field_type *field_types;
	char **field_titles;
	GtkEntry **field_entries;
	int field_count;
	void(*callback)(void*);
	void *userdata;
};

G_DEFINE_TYPE(EditorCreateResourceWindow, editor_create_resource_window, GTK_TYPE_APPLICATION_WINDOW);

static void
add_field_row(EditorCreateResourceWindow *window, int i) 
{
	char *title = window->field_titles[i];
	GtkBox *hbox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
	
	GtkWidget *label = gtk_label_new(title);
	GtkWidget *entry = gtk_entry_new();
	gtk_box_pack_start(hbox, label, FALSE, FALSE, 10);
	gtk_box_pack_start(hbox, entry, TRUE, TRUE, 10);
	gtk_box_pack_start(window->vbox, GTK_WIDGET(hbox), FALSE, FALSE, 10);
	gtk_widget_show_all(GTK_WIDGET(hbox));
	window->field_entries[i] = GTK_ENTRY(entry);
}

static void 
set_name(char *dest, char const *src) 
{
	strncpy(dest, src, MAX_NAME);
}
static void
set_char(char *dest, char const *src)
{
	*dest = *src;
}
static void
set_int(int *dest, char const *src)
{
	*dest = -1;
}

static void
clicked_done(GtkButton *button, EditorCreateResourceWindow *window)
{
	enum resource_field_type type;
	void *dest;
	char const *src;
	g_debug("clicked done!");
	for (int i = 0; i < window->field_count; i++) {
		type = window->field_types[i];
		dest = window->value_holders[i];
		src = gtk_entry_get_text(window->field_entries[i]);
		switch (type) {
			case RESOURCE_FIELD_NAME:
				set_name((char*)dest, src);
				break;
			case RESOURCE_FIELD_CHAR:
				set_char((char*)dest, src);
				break;
			case RESOURCE_FIELD_INT:
				set_int((int*)dest, src);
				break;
			default:
				g_debug("unhandled resource field type!");
		}
		
	}
	window->callback(window->userdata);
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
	g_debug("res window class init");
	G_OBJECT_CLASS(class)->finalize = editor_create_resource_window_finalize;

	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/create_resource_window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorCreateResourceWindow, vbox);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), clicked_done);
}

// Call like editor_create_resource_window_new(app, RESOURCE_FIELD_STRING, "name", RESOURCE_FIELD_INT, "hp", 0);
EditorCreateResourceWindow *
editor_create_resource_window_new(EditorApp *app, void(*callback)(void*), void *userdata, ...)
{
	g_debug("editor_create_resource_window_new...");
	int argcount = 0;
	EditorCreateResourceWindow *window = g_object_new(EDITOR_CREATE_RESOURCE_WINDOW_TYPE, "application", app, "title", "New Resource", NULL);
	g_debug("config...");
	va_list argp;
	va_start(argp, userdata);
	while (va_arg(argp, int) != (int)_RESOURCE_FIELD_END) {
		va_arg(argp, char*);
		va_arg(argp, void*);
		argcount += 1;
	}
	window->value_holders = malloc(argcount * sizeof (void *));
	window->field_types = malloc(argcount * sizeof (enum resource_field_type));
	window->field_titles = malloc(argcount * sizeof (char **));
	window->field_entries = malloc(argcount * sizeof (GtkEntry *));
	window->field_count = argcount;
	window->callback = callback;
	window->userdata = userdata;
	
	va_start(argp, userdata);
	for (int i = 0; i < argcount; i++) {
		window->field_types[i] = va_arg(argp, enum resource_field_type);
		char *title = va_arg(argp, char*);
		window->value_holders[i] = va_arg(argp, void*);
		window->field_titles[i] = malloc(strlen(title) + 1);
		strcpy(window->field_titles[i], title);
	}
	
	// Setup ui fields
	for (int i = 0; i < window->field_count; i++) {
		add_field_row(window, i);
	}

	return window;
}

