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

#define GAME_EDITOR_MAIN_WINDOW_TYPE (game_editor_main_window_get_type ())
G_DECLARE_FINAL_TYPE(GameEditorMainWindow, game_editor_main_window, GAME_EDITOR, MAIN_WINDOW, GtkApplicationWindow);

GameEditorMainWindow *game_editor_main_window_new(GameEditorApp *app);
void game_editor_main_window_open(GameEditorMainWindow *win, GFile *file);

#endif
