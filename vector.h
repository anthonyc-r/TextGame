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
