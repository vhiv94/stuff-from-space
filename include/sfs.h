#ifndef SFS_H
#define SFS_H

#define CORNFLOWER (Color) { 111, 144, 244, 255 }
#define SPACE (Color) { 33, 15, 75, 255 }
#define TRANSLUCENT (Color) { 245, 245, 245, 127 }

typedef struct Sprite
{
	Texture spriteImg;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} Sprite;

typedef struct Timer
{
	double startTime;
	double duration;
	char started;
} Timer;

typedef struct SpriteSheet
{
	Texture spriteImg;
	Timer timer;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} SpriteSheet;


typedef struct Circle
{
	Vector2 center;
	float radius;
} Circle;

#if 0
typedef struct Timer
{
  time_t startTime;
  time_t duration;
} Timer;
#endif


//int CheckTimeout(Timer timer);
int CheckAnimationTimer(Timer* timer);
void UpdateAnimation(SpriteSheet* sprite);
Vector2 GetNormalizedDirection(float rotation);

#endif // !SFS_H
