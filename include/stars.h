#ifndef STARS_H
#define STARS_H

typedef struct Star
{
	Vector2 position;
	float size;
	Color color;
} Star;

Star* LoadStars(int count);
void DrawStars(Star* stars, int count, Texture texture);

#endif // !STARS_H
