#ifndef ASTEROIDS_H
#define ASTEROIDS_H

typedef struct
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

void LoadAsteroids(Asteroid* asteroids);
void UpdateAsteroidPositions(Asteroid* asteroids, float dt);
void DrawAsteroids(Asteroid* asteroids, Sprite texture);

#endif // !ASTEROIDS_H