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

struct _EditorApp
{
	GtkApplication parent;
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
editor_app_startup(GApplication *app)
{
	G_APPLICATION_CLASS(editor_app_parent_class)->startup(app);

	GtkBuilder *builder = gtk_builder_new_from_resource("/rocks/colourful/textgame/menu.ui");
	GMenuModel *menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
	gtk_application_set_app_menu(GTK_APPLICATION(app), menu);
	g_object_unref(builder);
}

static void
editor_app_init(EditorApp *app)
{
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
