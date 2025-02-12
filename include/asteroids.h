#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "raylib.h"
#include "raymath.h"

#define CORNFLOWER (Color) { 111, 144, 244, 255 }
#define SPACE (Color) { 33, 15, 75, 255 }
#define TRANSLUCENT (Color) { 245, 245, 245, 127 }

typedef struct TexturePro
{
	Texture spriteImg;
	Rectangle srcRect;
	Rectangle destRect;
} TexturePro;

typedef struct Circle
{
	Vector2 center;
	float radius;
} Circle;

typedef struct Player
{
	TexturePro sprite;
	Circle collisionBody;
	Vector2 position;
	Vector2 direction;
	Vector2 velocity;
	float rotation;
	float speed;
} Player;


typedef struct Entity
{
	Circle collisionBody;
	Vector2 position;
	Vector2 direction;
	float speed;
} Entity;

float GetPlayerSpeed(float curSpeed);
Vector2 GetPlayerDirection(float rotation);
Entity* LoadAsteroids(int count);
void UpdateAsteroidPositions(Entity* asteroids, int count, float dt);
void DrawAsteroids(Entity* asteroids, int count, Texture texture);

#endif // ASTEROIDS_H
