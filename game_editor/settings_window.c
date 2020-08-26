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

#include "settings_window.h"
#include "settings.h"

struct _EditorSettingsWindow
{
	GtkApplicationWindow parent;
	GtkEntry *terminal_entry;
};

G_DEFINE_TYPE(EditorSettingsWindow, editor_settings_window, GTK_TYPE_APPLICATION_WINDOW);

static void
clicked_apply(GtkButton *button, gpointer user_data)
{
	EditorSettingsWindow *window = user_data;
	settings_set_terminal_emulator(gtk_entry_get_text(window->terminal_entry));
	g_debug("applied settings");
}

static void
editor_settings_window_init(EditorSettingsWindow *window)
{
	gtk_widget_init_template(GTK_WIDGET(window));
	gtk_entry_set_text(window->terminal_entry, settings_get_terminal_emulator());
}

static void
editor_settings_window_class_init(EditorSettingsWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/settings_window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorSettingsWindow,
		terminal_entry);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), clicked_apply);
}

EditorSettingsWindow *
editor_settings_window_new()
{
	return g_object_new(EDITOR_SETTINGS_WINDOW_TYPE, NULL);
}
