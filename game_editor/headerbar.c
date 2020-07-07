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
#include "headerbar.h"

struct _EditorHeaderbar
{
	GtkHeaderBar parent;
};

G_DEFINE_TYPE(EditorHeaderbar, editor_headerbar, GTK_TYPE_HEADER_BAR);

static void
editor_headerbar_init(EditorHeaderbar *window)
{
	gtk_widget_init_template(GTK_WIDGET(window));
}

static void
editor_headerbar_class_init(EditorHeaderbarClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),  "/rocks/colourful/textgame/headerbar.ui");
	
}

EditorHeaderbar *
editor_headerbar_new()
{
	return g_object_new(EDITOR_HEADERBAR_TYPE, NULL);
}
