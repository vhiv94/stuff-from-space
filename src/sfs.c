#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"
#include "sfs.h"
#include "player.h"
#include "lasers.h"
#include "asteroids.h"
#include "stars.h"

starCount = 500;
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
	SetRandomSeed(time(NULL));


	// initialize window
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1920, 1080, "Stuff From Space!");
	SetExitKey(0);

	// initialize audio
	InitAudioDevice();
	SetMasterVolume(0.8f);

	// resource_dir.h
	SearchAndSetResourceDir("resources");

	// initialize player
	Player player =
	{
		.collisionBody = {{ 28.0f, 22.0f }, 18.0f }, // center = { posX + 28, posY + 22 }
		.sprite.spriteImg = LoadTexture("spaceship.png"),
		.sprite.srcRect = { 0, 0, player.sprite.spriteImg.width, player.sprite.spriteImg.height },
		.sprite.destRect = { 0, 0, player.sprite.spriteImg.width / 2, player.sprite.spriteImg.height / 2 },
		.sprite.origin = { 28, 22 },
	};

	// initialize deathsplosion
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
		.timer.duration = 1.0 / 48.0,
		.srcRect = { 0, 0, death.spriteImg.width, death.spriteImg.height },
		.destRect = { 0, 0, death.spriteImg.width * 2, death.spriteImg.height * 2},
		.origin = { 48, 46 },
	};

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
	Sound laserSound = LoadSound("laser.wav");
	SetSoundVolume(laserSound, 0.5f);

	/*** background elements ***/
	// initialize earth
	SpriteSheet earth =
	{
			.spriteImg = LoadTexture("Earth-Like planet.png"),
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
	Music bgm = LoadMusicStream("music.wav");
	PlayMusicStream(bgm);
	SetMusicVolume(bgm, 0.6f);
#endif // NDEBUG

																/*********************
																*      GAMELOOP      *
																*********************/
	while (!WindowShouldClose())
	{
		// initialize death animation (once)
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

		// deltaT
		float dt = GetFrameTime();

		// hide cursor
		if (IsCursorOnScreen())
			HideCursor();

		// player inputs
		if (!player.dead)
			getPlayerInput(&player);
	
		// laser input and creation
		if (IsKeyPressed(KEY_SPACE) && !player.dead)
		{
			SetSoundPitch(laserSound, 0.01f * (float)GetRandomValue(95, 105));
			PlaySound(laserSound);
			lasers[laserCount++] = ShootLaser(player);
		}

		// update player
		if (!player.dead)
			UpdatePlayerPosition(&player, dt);
		
		// update asteroids
		UpdateAsteroidPositions(asteroids, dt);

		//update lasers
		UpdateLaserPositions(lasers, dt);

		// update camera
		camera.target = player.position;
		if (player.speed > 100)
			camera.zoom = 1.0f - ((player.speed - 100.0f) / (5.0f * MAX_SPEED));

		// animate planet
		if (CheckAnimationTimer(&earth.timer))
			UpdateAnimation(&earth);

		// animate death
		if (death.timer.started)
			if (CheckAnimationTimer(&death.timer))
				if (++deathFrame < 28)
					death.spriteImg = LoadTextureFromImage(deathSprites[deathFrame]);

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

		// set background color
		ClearBackground(SPACE);

		// TODO: fix with camera values
		// draw fps
		sprintf_s(&fps, 9, "%d FPS", GetFPS());
		DrawText(fps, player.position.x - 960.0f, player.position.y - 540.0f, 20, GREEN);

		// draw background
		DrawStars(stars, starSprite);
		DrawTexturePro(earth.spriteImg, earth.srcRect, earth.destRect, earth.origin, 0, RAYWHITE);

		// draw lasers
		if (laserCount)
			DrawLasers(lasers, laserSprite);

		// draw asteroids
		DrawAsteroids(asteroids, asteroidSprite);

		// draw player
		if (deathFrame < 16)
			DrawTexturePro(player.sprite.spriteImg, player.sprite.srcRect, player.sprite.destRect, player.sprite.origin, player.rotation, RAYWHITE);
#ifdef DEBUG
		if (!player.dead)
			DrawCircleV(player.collisionBody.center, player.collisionBody.radius, TRANSLUCENT);
#endif // DEBUG
		if (death.timer.started && deathFrame < 28)
			DrawTexturePro(death.spriteImg, death.srcRect, death.destRect, death.origin, 0, RAYWHITE);

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
	UnloadTexture(earth.spriteImg);
	UnloadTexture(death.spriteImg);

	// deallocate memory
	MemFree(asteroids);
	MemFree(lasers);

	// destroy the window and cleanup the OpenGL context
	CloseAudioDevice();
	CloseWindow();

	// return success!
	return 0;
}

boolean CheckAnimationTimer(Timer* timer)
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