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
	void *value_holders;
	enum resource_field_type *field_types;
	char **field_titles;
};

G_DEFINE_TYPE(EditorCreateResourceWindow, editor_create_resource_window, GTK_TYPE_APPLICATION_WINDOW);

static void
editor_create_resource_window_init(EditorCreateResourceWindow *window)
{
	gtk_widget_init_template(GTK_WIDGET(window));

	editor_app_add_ground(current_app, new_ground("water", '~'));
}

static void
editor_create_resource_window_class_init(EditorCreateResourceWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/create_resource_window.ui");
}

EditorCreateResourceWindow *
editor_create_resource_window_new(EditorApp *app, ...)
{
	return g_object_new(EDITOR_CREATE_RESOURCE_WINDOW_TYPE, "application", app, "title", "New Resource", NULL);
}

