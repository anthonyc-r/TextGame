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
#include "game_data.h"

int
main(int argc, char **argv) 
{
	struct entity ent = {"test ent", "a thing", 'e', 1231, SIZE_TINY };
	struct ground ground = { "mud", 'm' };
	struct creature creature = { "test name", "a creature", 100, 99, 5 };
	struct entity *ents[3] = { &ent, &ent, NULL };
	struct ground *grounds[3] = { &ground, &ground, NULL };
	struct creature *creatures[3] = { &creature, &creature, &creature };
	save_game_data("./savetest.dat", ents, grounds, creatures);
	
	
	current_app = editor_app_new();
	return g_application_run(G_APPLICATION(current_app), argc, argv);
}
