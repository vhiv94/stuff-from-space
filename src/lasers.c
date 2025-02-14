#include "raylib.h"
#include "sfs.h"
#include "player.h"
#include "lasers.h"

void ShootLaser(Player player, Laser* lasers, Sound sound)
{
	if (IsKeyPressed(KEY_SPACE) && !player.dead)
	{
		SetSoundPitch(sound, 0.01f * (float)GetRandomValue(95, 105));
		PlaySound(sound);
		lasers[laserCount++] = (Laser){ { player.position, 5.0f }, player.position, player.direction, player.rotation };
	}
}

void UpdateLaserPositions(Laser* lasers, float dt)
{
	if (laserCount)
	{
		for (int i = 0; i < laserCount; i++)
		{
			lasers[i].position.x += lasers[i].direction.x * LASER_SPEED * dt;
			lasers[i].position.y += lasers[i].direction.y * LASER_SPEED * dt;
			lasers[i].collisionBody.center = lasers[i].position;

			if (lasers[i].position.x > 4000 || lasers[i].position.x < -4000 || lasers[i].position.y > 2000 || lasers[i].position.y < -2000)
			{
				for (int j = i; j < laserCount; j++)
				{
					lasers[j] = lasers[j + 1];
				}
				laserCount--;
			}
		}
	}
}

void DrawLasers(Laser* lasers, Sprite texture)
{
	if (laserCount)
	{
		for (int i = 0; i < laserCount; i++)
		{
			texture.destRect.x = lasers[i].position.x;
			texture.destRect.y = lasers[i].position.y;
			DrawTexturePro(texture.spriteImg, texture.srcRect, texture.destRect, texture.origin, lasers[i].rotation, RAYWHITE);
		}
	}
}