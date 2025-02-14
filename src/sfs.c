#include <stdio.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"
#include "sfs.h"
#include "player.h"
#include "lasers.h"
#include "asteroids.h"
#include "stars.h"

char fps[10] = "\0";
starCount = 1000;
asteroidCount = 100;
laserCount = 0;

int main ()
{
															/***********************
															*      INITIALIZE      *
															*					 and				 *
															*      Load Assets		 *
															***********************/

	// variables
	SearchAndSetResourceDir("resources");
	SetRandomSeed(time(NULL));


	// initialize window
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1920, 1080, "Stuff From Space!");
	SetExitKey(0);

	// initialize audio
	InitAudioDevice();
	SetMasterVolume(0.5f);

	// initialize player
	Player player =
	{
		.sprite.spriteImg = LoadTexture("spaceship.png"),
		.sprite.srcRect = { 0, 0, player.sprite.spriteImg.width, player.sprite.spriteImg.height },
		.sprite.destRect = { 0, 0, player.sprite.spriteImg.width / 2, player.sprite.spriteImg.height / 2 },
		.sprite.origin = { 28, 22 },
		.collisionBody = {{ 28.0f, 22.0f }, 18.0f }, // center = { posX + 28, posY + 22 }
		.acceleration = 6.0f,
	};

	// initialize deathsplosion
	SpriteGroup death =
	{
		.frameTotal = 28,
		.timer.duration = 1.0 / 48.0,
	};
	LoadSpriteGroup(&death, 2.0f, "explosion", "png");
	Sound deathSound = LoadSound("Magical Interface 8-1.wav");
	SetSoundVolume(deathSound, 1.5f);

	// initialize asteroids
	Asteroid* asteroids = LoadAsteroids();
	Sprite asteroidSprite =
	{
			.spriteImg = LoadTexture("meteor.png"),
			.srcRect = { 0, 0, asteroidSprite.spriteImg.width, asteroidSprite.spriteImg.height },
	};

	// initialize lasers
	Laser* lasers = MemAlloc(100 * sizeof(Laser));
	Sprite laserSprite =
	{
			.spriteImg = LoadTexture("laser.png"),
			.srcRect = { 0, 0, laserSprite.spriteImg.width, laserSprite.spriteImg.height },
			.destRect = { 0, 0, laserSprite.spriteImg.width, laserSprite.spriteImg.height },
			.origin = { 5, 25 },
	};
	Sound laserSound = LoadSound("Sci-Fi Gun 1-1.wav");
	SetSoundVolume(laserSound, 0.5f);

	/*** background elements ***/
	// initialize earth
	SpriteSheet earth =
	{
			.sprite = LoadTexture("Earth-Like planet.png"),
			.timer.duration = 1.0 / 12.0,
			.srcRect = { 0, 0, 96, 96 },
			.destRect = { 200, 100, 384, 384 },
			.origin = { 192, 192 },
	};

	// initialize stars
	Star* stars = LoadStars();
	Texture starSprite = LoadTexture("star.png");

	// initialize camera
	Camera2D camera =
	{
		.offset = { 960, 540 },
		.zoom = 1.0f,
		.target = player.position,
	};

	// initialize bgm
#ifdef NDEBUG
	Music bgm = LoadMusicStream("Things From Space 1.mp3");
	PlayMusicStream(bgm);
	SetMusicVolume(bgm, 1.0f);
#endif // NDEBUG

																/*********************
																*      GAMELOOP      *
																*********************/
	while (!WindowShouldClose())
	{
		// initialize one-time events
		StartDeath(player, &death, deathSound);
		
																/********************
																*      UPDATES      * 
																********************/

		// deltaT
		float dt = GetFrameTime();

		// hide cursor
		if (IsCursorOnScreen())
			HideCursor();

		// player inputs
		GetPlayerInputs(&player, dt);
	
		// laser input and creation
		ShootLaser(player, lasers, laserSound);

		// update entities
		if (!player.dead)
			UpdatePlayerPosition(&player, dt);
		UpdateAsteroidPositions(asteroids, dt);
		UpdateLaserPositions(lasers, dt);

		// update camera
		camera.target = player.position;
		camera.zoom = 1.0f - (player.speed * 0.0325f);

		// update planet animation
		if (CheckAnimationTimer(&earth.timer))
			UpdateAnimationSS(&earth);

		// update player death animation
		if (death.timer.started)
			if (CheckAnimationTimer(&death.timer))
				UpdateAnimationSG(&death);

		// update bgm
#ifdef NDEBUG
		UpdateMusicStream(bgm);
#endif // NDEBUG

		// collision
		CheckCollisions(&player, asteroids, lasers);

																/***********************
																*      DRAW FRAME      * 
																***********************/

		BeginDrawing();
		BeginMode2D(camera);
		ClearBackground(SPACE);

		// draw background
		DrawStars(stars, starSprite);
		DrawTexturePro(earth.sprite, earth.srcRect, earth.destRect, earth.origin, 0, RAYWHITE);

		// draw entities
		DrawLasers(lasers, laserSprite);
		DrawAsteroids(asteroids, asteroidSprite);
		if (death.frameIndex < 16)
			DrawPlayer(player);
		DrawAnimationOnceSG(death);

		UpdateAndDrawFPS(camera);
		EndMode2D();
		EndDrawing();
	} // end of gameloop

																/********************
																*      CLEANUP      *
																********************/

	// unload assets
	UnloadSound(laserSound);
#ifdef NDEBUG
	StopMusicStream(bgm);
	UnloadMusicStream(bgm);
#endif // NDEBUG
	UnloadTexture(player.sprite.spriteImg);
	UnloadTexture(asteroidSprite.spriteImg);
	UnloadTexture(laserSprite.spriteImg);
	UnloadTexture(earth.sprite);
	UnloadTexture(death.curSprite);

	// deallocate memory
	MemFree(stars);
	MemFree(asteroids);
	MemFree(lasers);
	MemFree(death.spriteArr);

	// destroy the window and cleanup the OpenGL context
	CloseAudioDevice();
	CloseWindow();

	// return success!
	return 0;
}

																/**********************
																*      FUNCTIONS      *
																**********************/

boolean CheckAnimationTimer(Timer* timer)
{
  if (GetTime() >= timer->startTime + timer->duration)
  {
    timer->startTime += timer->duration;
    return 1;
  }
  return 0;
}

void LoadSpriteGroup(SpriteGroup* spriteGroup, float sizeFactor, char* dir, char* ft)
{
	spriteGroup->spriteArr = MemAlloc(spriteGroup->frameTotal * sizeof(Image));
	int bufCount = strlen(dir) + strlen(ft) + 6;
	char* filename = MemAlloc(bufCount * sizeof(char));
	for (int i = 0; i < spriteGroup->frameTotal; i++)
	{
		sprintf_s(filename, bufCount, "%s/%d.%s", dir, (i + 1), ft);
		spriteGroup->spriteArr[i] = LoadImage(filename);
	}
	MemFree(filename);
	spriteGroup->curSprite = LoadTextureFromImage(spriteGroup->spriteArr[0]);
	spriteGroup->frameIndex = 1;
	spriteGroup->srcRect = (Rectangle){ 0, 0, spriteGroup->curSprite.width, spriteGroup->curSprite.height };
	spriteGroup->destRect = (Rectangle){ 0, 0, sizeFactor * spriteGroup->curSprite.width, sizeFactor * spriteGroup->curSprite.height };
	spriteGroup->origin = (Vector2){ sizeFactor * spriteGroup->curSprite.width / 2, sizeFactor * spriteGroup->curSprite.height / 2 };
}

void UpdateAnimationSG(SpriteGroup* spriteGroup)
{
	if (spriteGroup->frameIndex < spriteGroup->frameTotal)
		spriteGroup->curSprite = LoadTextureFromImage(spriteGroup->spriteArr[spriteGroup->frameIndex++]);
	else if (spriteGroup->frameIndex == spriteGroup->frameTotal)
		spriteGroup->frameIndex++;
}

void DrawAnimationOnceSG(SpriteGroup spriteGroup)
{
	if (spriteGroup.timer.started && spriteGroup.frameIndex < (spriteGroup.frameTotal + 1))
		DrawTexturePro(spriteGroup.curSprite, spriteGroup.srcRect, spriteGroup.destRect, spriteGroup.origin, 0, RAYWHITE);
}

void UpdateAnimationSS(SpriteSheet* spriteSheet)
{
		if (spriteSheet->srcRect.x >= spriteSheet->sprite.width - spriteSheet->srcRect.width)
			spriteSheet->srcRect.x = 0;
		else
			spriteSheet->srcRect.x += spriteSheet->srcRect.width;
}

Vector2 GetDirectionFromRotation(float rotation)
{
	float radian = DEG2RAD * rotation;
	Vector2 direction = { sinf(radian), -cosf(radian) };
	return direction;
}

void UpdateAndDrawFPS(Camera2D camera)
{
	sprintf_s(fps, 10, "%d FPS", GetFPS());
	DrawText(fps, camera.target.x - (camera.offset.x / camera.zoom), camera.target.y - (camera.offset.y / camera.zoom), 20, GREEN);
}