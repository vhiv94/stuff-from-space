#ifndef ASTEROIDS_H
#define ASTEROIDS_H

typedef struct Asteroid
{
	Circle collisionBody;
	Vector2 spriteOrigin;
	Vector2 position;
	Vector2 direction;
	float speed;
	float rotation;
	float rotationSpeed;
	float size;
} Asteroid;

Asteroid* LoadAsteroids(int count);
void UpdateAsteroidPositions(Asteroid* asteroids, int count, float dt);
void DrawAsteroids(Asteroid* asteroids, int count, TexturePro texture);

#endif // !ASTEROIDS_H