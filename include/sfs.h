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
	Timer timer;
	Texture sprite;
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 origin;
} SpriteSheet;

typedef struct SpriteGroup
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
int asteroidCount;
int laserCount;

boolean CheckAnimationTimer(Timer* timer);
void LoadSpriteGroup(SpriteGroup* spriteGroup, float sizeFactor, char* dir, char* ft);
void UpdateAnimationSG(SpriteGroup* spriteGroup);
void DrawAnimationOnceSG(SpriteGroup spriteGroup);
void UpdateAnimationSS(SpriteSheet* spriteSheet);
Vector2 GetDirectionFromRotation(float rotation);
void UpdateAndDrawFPS(Camera2D camera);

#endif // !SFS_H
