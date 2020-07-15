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

#ifndef _GAME_EDITOR_H
#define _GAME_EDITOR_H

#include <gtk/gtk.h>

#define EDITOR_APP_TYPE (editor_app_get_type ())
G_DECLARE_FINAL_TYPE(EditorApp, editor_app, EDITOR, APP,  GtkApplication)

EditorApp *current_app;

EditorApp *editor_app_new();
GtkTreeModel *editor_app_get_entity_tree_model(EditorApp *app);
GtkTreeModel *editor_app_get_creature_tree_model(EditorApp *app);
GtkTreeModel *editor_app_get_ground_tree_model(EditorApp *app);

#endif

