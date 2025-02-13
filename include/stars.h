#ifndef STARS_H
#define STARS_H

typedef struct Star
{
	Vector2 position;
	float size;
	Color color;
} Star;

Star* LoadStars();
void DrawStars(Star* stars, Texture texture);

#endif // !STARS_H
