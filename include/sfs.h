#ifndef SFS_H
#define SFS_H

#define CORNFLOWER (Color) { 111, 144, 244, 255 }
#define SPACE (Color) { 33, 15, 75, 255 }
#define TRANSLUCENT (Color) { 245, 245, 245, 127 }

typedef unsigned char boolean;

typedef struct Circle
{
	Vector2 center;
	float radius;
} Circle;

typedef struct Timer
{
	double startTime;
	double duration;
	boolean started;
} Timer;

typedef struct Sprite
{
	Texture spriteImg;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} Sprite;

typedef struct SpriteSheet
{
	Texture spriteImg;
	Timer timer;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} SpriteSheet;

int starCount;
int asteroidCount;
int laserCount;
char fps[9];

boolean CheckAnimationTimer(Timer* timer);
void UpdateAnimation(SpriteSheet* sprite);
Vector2 GetNormalizedDirection(float rotation);

#endif // !SFS_H
