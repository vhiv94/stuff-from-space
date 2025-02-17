#ifndef LASERS_H
#define LASERS_H

#define LASER_SPEED 1200.0f

typedef struct
{
	Circle collisionBody;
	Vector2 position;
	Vector2 direction;
	float rotation;
} Laser;

void ShootLaser(Player player, Laser* lasers, Sound sound);
void UpdateLaserPositions(Laser* lasers, float dt);
void DrawLasers(Laser* lasers, Sprite texture);

#endif // !LASERS_H
