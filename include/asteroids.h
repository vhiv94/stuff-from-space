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

Asteroid* LoadAsteroids();
void UpdateAsteroidPositions(Asteroid* asteroids, float dt);
void DrawAsteroids(Asteroid* asteroids, Sprite texture);
void CheckCollisions(Player* player, Asteroid* asteroids, Laser* lasers);

#endif // !ASTEROIDS_H