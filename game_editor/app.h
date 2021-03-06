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

struct ground;
struct creature;
struct entity;
enum size_type;
struct cell;

#define EDITOR_APP_TYPE (editor_app_get_type ())
G_DECLARE_FINAL_TYPE(EditorApp, editor_app, EDITOR, APP,  GtkApplication)

EditorApp *current_app;

EditorApp *editor_app_new();
// remember to free the items when removed from these lists...
GtkTreeModel *editor_app_get_entity_tree_model(EditorApp *app);
GtkTreeModel *editor_app_get_creature_tree_model(EditorApp *app);
GtkTreeModel *editor_app_get_ground_tree_model(EditorApp *app);

void editor_app_add_ground(EditorApp *app, struct ground *ground);
void editor_app_add_creature(EditorApp *app, struct creature *creature);
void editor_app_add_entity(EditorApp *app, struct entity *entity);
struct map *editor_app_get_map(EditorApp *app);

int editor_app_update_ground(EditorApp *app, struct ground *ground, const char *name, const char icon);
int editor_app_update_entity(EditorApp *app, struct entity *entity, const char *name, const char *desc, const char icon, int weight, enum size_type size_type);
int editor_app_update_creature(EditorApp *app, struct creature *creature, const char *name, const char *desc, int health, int tp, int inventory_size);
int editor_app_update_cell(EditorApp *app, struct cell *cell, struct entity *entity, struct creature *creature, struct ground *ground);
void editor_app_run_game(EditorApp *app);
#endif

