/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "sfs.h"
#include "player.h"
#include "lasers.h"
#include "asteroids.h"
#include "stars.h"


int main ()
{
	int asteroidCount = 100;
	int laserCount = 0;
	int frameCount = 0;
	char fps[9] = {'\0'};

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1920, 1080, "Asteroids");
	SetExitKey(KEY_ESCAPE);
	InitAudioDevice();
	SetMasterVolume(0.8f);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	SetRandomSeed(time(NULL));

	// Load Entities and Assets

	Player player =
	{
		.collisionBody = {{ 28.0f, 22.0f }, 18.0f }, // center = { posX + 28, posY + 22 }
		.sprite.spriteImg = LoadTexture("spaceship.png"),
		.sprite.srcRect = { 0, 0, player.sprite.spriteImg.width, player.sprite.spriteImg.height },
		.sprite.destRect = { 0, 0, player.sprite.spriteImg.width / 2, player.sprite.spriteImg.height / 2 },
		.sprite.origin = { 28, 22 },
	};

	Image deathSprites[28] = {0};
	int deathFrame = -1;
	char deathSpritesFileName[17];
	for (int i = 1; i < 29; i++)
	{
		sprintf_s(deathSpritesFileName, 17, "explosion/%d.png", i);
		deathSprites[i - 1] = LoadImage(deathSpritesFileName);
	}
	SpriteSheet death =
	{
		.spriteImg = LoadTextureFromImage(deathSprites[0]),
		.timer.duration = 1.0 / 24.0,
		.srcRect = { 0, 0, death.spriteImg.width, death.spriteImg.height },
		.destRect = { 0, 0, death.spriteImg.width * 2, death.spriteImg.height * 2},
		.origin = { 48, 46 },
	};

	// asteroid assets
	Asteroid* asteroids = LoadAsteroids(asteroidCount);
	Sprite asteroidSprite =
	{
			.spriteImg = LoadTexture("meteor.png"),
			.srcRect = { 0, 0, asteroidSprite.spriteImg.width, asteroidSprite.spriteImg.height },
	};

	// laser assets
	Laser* lasers = MemAlloc(100 * sizeof(Laser));
	Sprite laserSprite =
	{
			.spriteImg = LoadTexture("laser.png"),
			.srcRect = { 0, 0, laserSprite.spriteImg.width, laserSprite.spriteImg.height },
			.destRect = { 0, 0, laserSprite.spriteImg.width, laserSprite.spriteImg.height },
			.origin = { 5, 25 },
	};
	Sound laserSound = LoadSound("laser.wav");
	SetSoundVolume(laserSound, 0.5f);

	// background elements
	SpriteSheet earth =
	{
			.spriteImg = LoadTexture("Earth-Like planet.png"),
			.timer.duration = 1.0 / 12.0,
			.srcRect = { 0, 0, 96, 96 },
			.destRect = { 200, 100, 384, 384 },
			.origin = { 192, 192 },
	};

	Star* stars = LoadStars(500);
	Texture starSprite = LoadTexture("star.png");

	Camera2D camera =
	{
		.offset = { 960, 540 },
		.zoom = 1.0f,
		.target = player.position,
	};


#ifdef NDEBUG
	Music bgm = LoadMusicStream("music.wav");
	PlayMusicStream(bgm);
	SetMusicVolume(bgm, 0.6f);
#endif // NDEBUG

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// increment frame counter as a representation of which frame is currently being calculated
		frameCount++;

		// inputs
		if (IsCursorOnScreen())
			HideCursor();

		getPlayerInput(&player);
	
		if (!player.dead)
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				SetSoundPitch(laserSound, 0.01f * (float)GetRandomValue(95, 105));
				PlaySound(laserSound);
				lasers[laserCount++] = ShootLaser(player);
			}
		}

		if (player.dead && !death.timer.started)
		{
			death.timer.started = 1;
			death.timer.startTime = GetTime();
			death.destRect.x = player.position.x;
			death.destRect.y = player.position.y;
		}

		/********************
		*      UPDATES      * 
		********************/

#ifdef NDEBUG
		UpdateMusicStream(bgm);
#endif // NDEBUG

		// deltaT
		float dt = GetFrameTime();

		UpdatePlayerPosition(&player, dt);
		
		camera.target = player.position;
		if (player.speed > 100)
			camera.zoom = 1.0f - ((player.speed - 100.0f) / (5.0f * MAX_SPEED));

		UpdateAsteroidPositions(asteroids, asteroidCount, dt);
		UpdateLaserPositions(lasers, &laserCount, dt);
		if (CheckAnimationTimer(&earth.timer))
			UpdateAnimation(&earth);

		if (death.timer.started)
			if (CheckAnimationTimer(&death.timer))
				if (++deathFrame < 28)
					death.spriteImg = LoadTextureFromImage(deathSprites[deathFrame]);

		// collision
		CheckCollisions(&player, asteroids, &asteroidCount, lasers, &laserCount);

		// drawing
		BeginDrawing();
		BeginMode2D(camera);

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(SPACE);

		// draw some text using the default font
		sprintf_s(&fps, 9, "%d FPS", GetFPS());
		DrawText(fps, player.position.x - 960.0f, player.position.y - 540.0f, 20, GREEN);

		// draw our texture to the screen
		//DrawCircleV((Vector2){ 0, 0 }, 30, WHITE);
		DrawStars(stars, 500, starSprite);
		DrawTexturePro(earth.spriteImg, earth.srcRect, earth.destRect, earth.origin, 0, RAYWHITE);
		DrawLasers(lasers, laserCount, laserSprite);
		DrawAsteroids(asteroids, asteroidCount, asteroidSprite);
		if (deathFrame < 16)
			DrawTexturePro(player.sprite.spriteImg, player.sprite.srcRect, player.sprite.destRect, player.sprite.origin, player.rotation, RAYWHITE);
		if (death.timer.started && deathFrame < 28)
			DrawTexturePro(death.spriteImg, death.srcRect, death.destRect, death.origin, 0, RAYWHITE);

#ifdef DEBUG
		if (!player.dead)
			DrawCircleV(player.collisionBody.center, player.collisionBody.radius, TRANSLUCENT);
#endif // DEBUG

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndMode2D();
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadSound(laserSound);
#ifdef NDEBUG
	StopMusicStream(bgm);
	UnloadMusicStream(bgm);
#endif // NDEBUG
	UnloadTexture(player.sprite.spriteImg);
	UnloadTexture(asteroidSprite.spriteImg);
	UnloadTexture(laserSprite.spriteImg);
	UnloadTexture(earth.spriteImg);
	UnloadTexture(death.spriteImg);
	MemFree(asteroids);
//	MemFree(lasers);

	// destroy the window and cleanup the OpenGL context
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

#if 0
int CheckTimeout(Timer timer)
{
  if (time(0) >= timer.startTime + timer.duration)
    return 1;
  else
    return 0;
}
#endif

int CheckAnimationTimer(Timer* timer)
{
  if (GetTime() >= timer->startTime + timer->duration)
  {
    timer->startTime += timer->duration;
    return 1;
  }
  return 0;
}

void UpdateAnimation(SpriteSheet* sprite)
{
		if (sprite->srcRect.x >= sprite->spriteImg.width - sprite->srcRect.width)
			sprite->srcRect.x = 0;
		else
			sprite->srcRect.x += sprite->srcRect.width;
}

Vector2 GetNormalizedDirection(float rotation)
{
	float radian = DEG2RAD * rotation;
	Vector2 direction = { sinf(radian), -cosf(radian) };
	direction = Vector2Normalize(direction);
	return direction;
}