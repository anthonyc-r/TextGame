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
#include "properties_view.h"

struct _EditorPropertiesView
{
	GtkBox parent;
};

G_DEFINE_TYPE(EditorPropertiesView, editor_properties_view, GTK_TYPE_BOX);

static void
editor_properties_view_init(EditorPropertiesView *view)
{

}

static void
editor_properties_view_class_init(EditorHeaderbarClass *class)
{

	
}

EditorPropertiesView *
editor_properties_view_new()
{
	return g_object_new(EDITOR_PROPERTIES_VIEW_TYPE, NULL);
}
