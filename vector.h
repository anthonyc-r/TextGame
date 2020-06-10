/*
Copyright 2017-2020 Anthony Cohn-Richardby

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


#ifndef _VECTOR_H
#define _VECTOR_H

struct vector2f {
	float x;
	float y;
};

struct vector2i {
	int x;
	int y;
};

struct vector2f new_vector2f(float x, float y);
struct vector2f vector2f_angle(struct vector2f origin, float magnitude, float angle);
struct vector2f vector2f_along(struct vector2f origin, struct vector2f destination, float fraction);
struct vector2i vector2f_round(struct vector2f vec);

struct vector2f vector2f_i(struct vector2i vec);
struct vector2i vector2i_f(struct vector2f vec);
#endif
