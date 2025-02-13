#include "raylib.h"
#include "raymath.h"
#include "sfs.h"
#include "player.h"
#include "lasers.h"
#include "asteroids.h"

Asteroid* LoadAsteroids()
{
	Asteroid* asteroids = (Asteroid*)MemAlloc(asteroidCount * sizeof(Asteroid));
	for (int i = 0; i < asteroidCount; i++)
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

void UpdateAsteroidPositions(Asteroid* asteroids, float dt)
{
	for (int i = 0; i < asteroidCount; i++)
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

void DrawAsteroids(Asteroid* asteroids, Sprite texture)
{
	for (int i = 0; i < asteroidCount; i++)
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

void CheckCollisions(Player* player, Asteroid* asteroids, Laser* lasers)
{
	for (int i = 0; i < asteroidCount; i++)
	{

		if (laserCount)
		{
			for (int j = 0; j < laserCount; j++)
			{
				if (CheckCollisionCircles(asteroids[i].collisionBody.center, asteroids[i].collisionBody.radius, lasers[j].collisionBody.center, lasers[j].collisionBody.radius))
				{
				  // destroy laser
					for (int k = j; k < laserCount; k++)
					{
						lasers[k] = lasers[k + 1];
					}
					laserCount--;

					// destroy asteroid
					//asteroid[i].dead = 1;
					for (int k = i; k < asteroidCount; k++)
					{
						asteroids[k] = asteroids[k + 1];
					}
					asteroidCount--;
				}
			}
		}

		if (!player->dead)
		{
			if (CheckCollisionCircles(asteroids[i].collisionBody.center, asteroids[i].collisionBody.radius, player->collisionBody.center, player->collisionBody.radius))
			{
				player->speed = 0;
				player->dead = 1;
			}
		}
	}
}
