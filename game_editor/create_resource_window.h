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

#ifndef _NEW_RESOURCE_WINDOW_H
#define _NEW_RESOURCE_WINDOW_H

#include <gtk/gtk.h>

enum resource_field_type {
	_RESOURCE_FIELD_END,
	RESOURCE_FIELD_STRING,
	RESOURCE_FIELD_CHAR,
	RESOURCE_FIELD_INT
};

#define EDITOR_CREATE_RESOURCE_WINDOW_TYPE (editor_create_resource_window_get_type ())
G_DECLARE_FINAL_TYPE(EditorCreateResourceWindow, editor_create_resource_window, EDITOR, CREATE_RESOURCE_WINDOW, GtkApplicationWindow);

EditorCreateResourceWindow *editor_create_resource_window_new(EditorApp *app, ...);

#endif
