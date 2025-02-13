#include "raylib.h"
#include "sfs.h"
#include "player.h"
#include "lasers.h"

Laser ShootLaser(Player player)
{
	Laser newLaser =
	{
	  .position = player.position,
		.direction = player.direction,
		.collisionBody = { player.position, 5.0f },
		.rotation = player.rotation,
	};
	return newLaser;
}

void UpdateLaserPositions(Laser* lasers, int count, float dt)
{
		if (count == 0)
				return;
		for (int i = 0; i < count; i++)
		{
				lasers[i].position.x += lasers[i].direction.x * LASER_SPEED * dt;
				lasers[i].position.y += lasers[i].direction.y * LASER_SPEED * dt;
		}
}

void DrawLasers(Laser* lasers, int count, TexturePro texture)
{
		if (count == 0)
				return;
		for (int i = 0; i < count; i++)
		{
				texture.destRect.x = lasers[i].position.x;
				texture.destRect.y = lasers[i].position.y;
				DrawTexturePro(texture.spriteImg, texture.srcRect, texture.destRect, texture.origin, lasers[i].rotation, RAYWHITE);
		}

}