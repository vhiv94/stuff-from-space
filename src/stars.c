#include "raylib.h"
#include "stars.h"

Color colors[7] = 
{ 
	{ 147, 233, 190, 127 }, // seafoam
	{ 200, 162, 200, 127 }, // lilac
	{ 135, 206, 235, 127 }, // sky
	{ 255, 244, 188, 127 }, // mimosa (yellow)
	{ 169, 64, 100, 127 }, // rouge
	{ 252, 108, 133, 127 }, // watermelon
	{ 245, 245, 245, 127 }, // raywhite
};

Star* LoadStars(int count)
{
	Star* stars = (Star*)MemAlloc(count * sizeof(Star));
	for (int i = 0; i < count; i++)
	{
		stars[i].position = (Vector2){ GetRandomValue(-4000, 4000), GetRandomValue(-2000, 2000) };
		stars[i].size = (float)GetRandomValue(1, 3) * 0.5f;
		stars[i].color = colors[GetRandomValue(0, 6)];
	}
	return stars;
}

void DrawStars(Star* stars, int count, Texture texture)
{
	for (int i = 0; i < count; i++)
	{
		DrawTextureEx(texture, stars[i].position, 0, stars[i].size, stars[i].color);
	}
}

