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

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <gtk/gtk.h>

#include "app.h"

enum active_item_type {
	ACTIVE_ITEM_NONE,
	ACTIVE_ITEM_GROUND,
	ACTIVE_ITEM_CREATURE,
	ACTIVE_ITEM_ENTITY,
	ACTIVE_ITEM_CELL
};


#define GROUND_RES_ID "ground_res_id"
#define ENTITY_RES_ID "entity_res_id"
#define CREATURE_RES_ID "creature_res_id"

#define EDITOR_MAIN_WINDOW_TYPE (editor_main_window_get_type ())
G_DECLARE_FINAL_TYPE(EditorMainWindow, editor_main_window, EDITOR, MAIN_WINDOW, GtkApplicationWindow);

EditorMainWindow *editor_main_window_new(EditorApp *app);

#endif
