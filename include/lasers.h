#ifndef LASERS_H
#define LASERS_H

#define LASER_SPEED 1200.0f

typedef struct Laser
{
	Circle collisionBody;
	Vector2 position;
	Vector2 direction;
	float rotation;
} Laser;

Laser ShootLaser(Player player);
void UpdateLaserPositions(Laser* lasers, int* count, float dt);
void DrawLasers(Laser* lasers, int count, Sprite texture);

#endif // !LASERS_H
