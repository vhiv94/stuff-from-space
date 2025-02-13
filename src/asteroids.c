#include "raylib.h"
#include "raymath.h"
#include "sfs.h"
#include "player.h"
#include "asteroids.h"

Asteroid* LoadAsteroids(int count)
{
	Asteroid* asteroids = (Asteroid*)MemAlloc(count * sizeof(Asteroid));
	for (int i = 0; i < count; i++)
	{
			asteroids[i].position = (Vector2){ GetRandomValue(-4000, 4000), GetRandomValue(-2000, 2000) };
			asteroids[i].direction = (Vector2){ (float)GetRandomValue(-100, 100) / 100.0f, (float)GetRandomValue(-100, 100) / 100.0f };
			asteroids[i].direction = Vector2Normalize(asteroids[i].direction);
			asteroids[i].speed = (float)GetRandomValue(20, 100);
			asteroids[i].rotation = (float)GetRandomValue(0, 359);
			asteroids[i].rotationSpeed = (float)GetRandomValue(0, 10) * 5.0f;
			asteroids[i].size = (float)GetRandomValue(1, 5);
			asteroids[i].spriteOrigin = (Vector2){ 50 / asteroids[i].size, 40 / asteroids[i].size };
			asteroids[i].collisionBody.radius = 45 / asteroids[i].size;
			asteroids[i].collisionBody.center = asteroids[i].position;
	}
	return asteroids;
}

void UpdateAsteroidPositions(Asteroid* asteroids, int count, float dt)
{
	for (int i = 0; i < count; i++)
	{
		if (asteroids[i].position.x <= -4000 || asteroids[i].position.x >= 4000)
			asteroids[i].position.x *= -1;
		if (asteroids[i].position.y <= -2000 || asteroids[i].position.y >= 2000)
			asteroids[i].position.y *= -1;
		asteroids[i].position.x += asteroids[i].direction.x * asteroids[i].speed * dt;
		asteroids[i].position.y += asteroids[i].direction.y * asteroids[i].speed * dt;
		asteroids[i].rotation += asteroids[i].rotationSpeed * dt;
		asteroids[i].collisionBody.center = asteroids[i].position;

	}
}

int CheckAsteroidCollisions(Asteroid* asteroids, int count, Player player)
{
		for (int i = 0; i < count; i++)
		{
				if (CheckCollisionCircles(asteroids[i].collisionBody.center, asteroids[i].collisionBody.radius, player.collisionBody.center, player.collisionBody.radius))
				{
						//initialize player destruction
						
						//if size == 5, initialize asteroid destruction
						if (asteroids[i].size == 5)
						{

						}
				}
		}
}

void DrawAsteroids(Asteroid* asteroids, int count, TexturePro texture)
{
	for (int i = 0; i < count; i++)
	{
		texture.destRect.x = asteroids[i].position.x;
		texture.destRect.y = asteroids[i].position.y;
		texture.destRect.width = (float)texture.spriteImg.width / asteroids[i].size;
		texture.destRect.height = (float)texture.spriteImg.height / asteroids[i].size;
		DrawTexturePro(texture.spriteImg, texture.srcRect, texture.destRect, asteroids[i].spriteOrigin, asteroids[i].rotation, RAYWHITE);
#ifdef DEBUG
		DrawCircleV(asteroids[i].collisionBody.center, asteroids[i].collisionBody.radius, TRANSLUCENT);
#endif // DEBUG

	}
}
