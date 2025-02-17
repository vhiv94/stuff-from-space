#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "stars.h"

#define CORNFLOWER (Color) { 111, 144, 244, 255 }
#define SPACE (Color) { 33, 15, 75, 255 }
#define TRANSLUCENT (Color) { 245, 245, 245, 127 }

typedef unsigned char boolean;

typedef struct
{
	Vector2 center;
	float radius;
} Circle;

typedef struct
{
	Rectangle rect;
	char* text;
} Button;

typedef struct
{
	double startTime;
	double duration;
	boolean started;
} Timer;

typedef struct
{
	Texture spriteImg;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} Sprite;

typedef struct
{
	Timer timer;
	Texture sprite;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} SpriteSheet;

typedef struct
{
	Timer timer;
	Image* spriteArr;
	Texture curSprite;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
	int frameTotal;
	int frameIndex;
} SpriteGroup;

int starCount;
SpriteSheet earth;
Star* stars;
Texture starSprite;

void MainLoop(void);
boolean CheckAnimationTimer(Timer* timer);
void LoadSpriteGroup(SpriteGroup* spriteGroup, float sizeFactor, char* dir, char* ft);
void UpdateAnimationSG(SpriteGroup* spriteGroup);
void DrawAnimationOnceSG(SpriteGroup spriteGroup);
void UpdateAnimationSS(SpriteSheet* spriteSheet);

#endif // !MAINLOOP_H
