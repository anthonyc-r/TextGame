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

#ifndef _PROPERTIES_VIEW_H
#define _PROPERTIES_VIEW_H

#include <gtk/gtk.h>

#include "app.h"

#define EDITOR_PROPERTIES_VIEW_TYPE (editor_properties_view_get_type ())
G_DECLARE_FINAL_TYPE(EditorPropertiesView, editor_properties_view, EDITOR, PROPERTIES_VIEW, GtkApplicationWindow);

EditorPropertiesView *editor_properties_view_new();
void editor_properties_view_set_item(EditorPropertiesView *view, enum active_item_type type, void *item);

#endif
