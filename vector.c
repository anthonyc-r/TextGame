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


#include "vector.h"
#include <math.h>

struct vector2f
new_vector2f(float x, float y)
{
	struct vector2f vec;
	vec.x = x;
	vec.y = y;
	return vec;
}

struct vector2f
vector2f_angle(struct vector2f origin, float magnitude, float angle)
{
	struct vector2f ret;
	float dx = magnitude * cos(angle);
	float dy = magnitude * sin(angle);
	ret.x = origin.x + dx;
	ret.y = origin.y + dy;
	return ret;
}

struct vector2f
vector2f_along(struct vector2f origin, struct vector2f destination, float fraction)
{
	struct vector2f ret;
	float dx = fraction * (destination.x - origin.x);
	float dy = fraction * (destination.y - origin.y);
	ret.x = origin.x + dx;
	ret.y = origin.y + dy;
	return ret;
}

struct vector2i
vector2f_round(struct vector2f vec)
{
	struct vector2i ret;
	ret.x = (int) round(vec.x);
	ret.y = (int) round(vec.y);
	return ret;
}

struct vector2f 
vector2f_i(struct vector2i vec)
{
	struct vector2f ret;
	ret.x = (float) vec.x;
	ret.y = (float) vec.y;
	return ret;
}
