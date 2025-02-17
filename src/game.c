#include <stdio.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "raygui.h"
#include "raymath.h"
#include "mainloop.h"
#include "game.h"
#include "player.h"
#include "lasers.h"
#include "asteroids.h"
#include "stars.h"

char fps[10] = "\0";
asteroidCount = 100;
laserCount = 0;
Timer gameoverTimer = { .duration = 3.0, };
boolean quitToMainMenu = 0;
Player player = { 0 };
Asteroid* asteroids = { 0 };
Laser* lasers = { 0 };
SpriteGroup death =
{
	.frameTotal = 28,
	.timer.duration = 1.0 / 48.0,
};
Sound deathSound;
Button retryButton = { { 860, 530, 200, 20}, "Restart Game" };
Button quitButton = { { 860, 570, 200, 20}, "Quit to Main Menu" };

static void UpdateAndDrawFPS(Camera2D camera);
static void CheckCollisions(void);
static void Gameover(void);

static void initializeGameboard(void)
{
	// set/reset player position
	player.position = (Vector2){ 960, 540 };
	player.sprite.destRect.x = player.position.x;
	player.sprite.destRect.y = player.position.y;
	player.collisionBody.center.x = player.position.x + 28.0f;
	player.collisionBody.center.y = player.position.y + 22.0f;
	player.acceleration = 3.0f;

	// set/reset asteroids
	if (asteroidCount < 100)
	{
		asteroidCount = 100;
		LoadAsteroids(asteroids);
	}
}

void RunGame(void)
{
	SetRandomSeed(time(NULL));

	/***   load game assets   ***/
	// load player sprte and set constants
	player.sprite.spriteImg = LoadTexture("spaceship.png");
	player.sprite.srcRect.width = player.sprite.spriteImg.width;
  player.sprite.srcRect.height = player.sprite.spriteImg.height;
	player.sprite.destRect.width = player.sprite.spriteImg.width / 2;
	player.sprite.destRect.height = player.sprite.spriteImg.height / 2;
	player.sprite.origin = (Vector2){ 28, 22 };
	player.collisionBody.radius = 18.0f;

	// load deathsplosion sprite and sound
	LoadSpriteGroup(&death, 2.0f, "explosion", "png");
	deathSound = LoadSound("Magical Interface 8-1.wav");
	SetSoundVolume(deathSound, 1.5f);

	// load asteroid sprte and set constants
	Sprite asteroidSprite =
	{
			.spriteImg = LoadTexture("meteor.png"),
			.srcRect = { 0, 0, asteroidSprite.spriteImg.width, asteroidSprite.spriteImg.height },
	};
  asteroids = (Asteroid*)MemAlloc(asteroidCount * sizeof(Asteroid));
	LoadAsteroids(asteroids);

	// initialize player and asteroid positions
	initializeGameboard();

	// load laser sprte and sound
	lasers = MemAlloc(100 * sizeof(Laser));
	Sprite laserSprite =
	{
			.spriteImg = LoadTexture("laser.png"),
			.srcRect = { 0, 0, laserSprite.spriteImg.width, laserSprite.spriteImg.height },
			.destRect = { 0, 0, laserSprite.spriteImg.width, laserSprite.spriteImg.height },
			.origin = { 5, 25 },
	};
	Sound laserSound = LoadSound("Sci-Fi Gun 1-1.wav");
	SetSoundVolume(laserSound, 0.5f);

	// initialize camera
	Camera2D camera =
	{
		.offset = { 960, 540 },
		.zoom = 1.0f,
		.target = player.position,
	};

	// initialize bgm
#ifdef NDEBUG
	Music bgm = LoadMusicStream("Things From Space 3.mp3");
	PlayMusicStream(bgm);
	SetMusicVolume(bgm, 1.0f);
#endif // NDEBUG

																/*********************
																*      GAMELOOP      *
																*********************/

	while (!WindowShouldClose() && !quitToMainMenu)
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
		GetPlayerInputs(&player);
	
		// laser input and creation
		ShootLaser(player, lasers, laserSound);

		// update entities
		UpdatePlayerPosition(&player, dt);
		UpdateAsteroidPositions(asteroids, dt);
		UpdateLaserPositions(lasers, dt);

		// update camera
		camera.target = player.position;
		camera.zoom = 1.0f - (0.15f * player.speed / MAX_SPEED);
		//camera.zoom = 1.0f - (player.speed * 0.0325f);

		// update planet animation
		if (CheckAnimationTimer(&earth.timer))
			UpdateAnimationSS(&earth);

		// update player death animation
		if (death.timer.started)
			if (CheckAnimationTimer(&death.timer))
				UpdateAnimationSG(&death);

		// start gameover timer
		if (death.frameIndex > death.frameTotal)
		{
			gameoverTimer.startTime = GetTime();
			gameoverTimer.started = 1;
		}

		// update bgm
#ifdef NDEBUG
		UpdateMusicStream(bgm);
#endif // NDEBUG

		// collision
		CheckCollisions();

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

		// set gameover screen
		if (gameoverTimer.started)
			if (GetTime() >= gameoverTimer.started + gameoverTimer.duration)
			{
				if (IsCursorOnScreen)
					ShowCursor();
				Gameover();
			}

		UpdateAndDrawFPS(camera);
		EndMode2D();
		EndDrawing();
	} // end of gameloop

																/********************
																*      CLEANUP      *
																********************/

	// unload assets
	UnloadSound(laserSound);
	UnloadTexture(player.sprite.spriteImg);
	UnloadTexture(asteroidSprite.spriteImg);
	UnloadTexture(laserSprite.spriteImg);
	UnloadTexture(death.curSprite);

	// deallocate memory
	MemFree(asteroids);
	MemFree(lasers);
	MemFree(death.spriteArr);

	// stop and unload bgm
#ifdef NDEBUG
	StopMusicStream(bgm);
	UnloadMusicStream(bgm);
#endif // NDEBUG
}

																/**********************
																*      FUNCTIONS      *
																**********************/

void UpdateAndDrawFPS(Camera2D camera)
{
	sprintf_s(fps, 10, "%d FPS", GetFPS());
	DrawText(fps, camera.target.x - (camera.offset.x / camera.zoom), camera.target.y - (camera.offset.y / camera.zoom), 20, GREEN);
}

static boolean CheckCollisionCirclesC(Circle c1, Circle c2)
{
	return CheckCollisionCircles(c1.center, c1.radius, c2.center, c2.radius);
}

void CheckCollisions(void)
{
	for (int i = 0; i < asteroidCount; i++)
	{

		if (laserCount)
		{
			for (int j = 0; j < laserCount; j++)
			{
				if (CheckCollisionCirclesC(asteroids[i].collisionBody, lasers[j].collisionBody))
				{
				  // destroy laser
					for (int k = j; k < laserCount; k++)
					{
						lasers[k] = lasers[k + 1];
					}
					laserCount--;

					// destroy asteroid
					//asteroid[i].dead = 1;
					for (int k = i; k < asteroidCount; k++)
					{
						asteroids[k] = asteroids[k + 1];
					}
					asteroidCount--;
				}
			}
		}

		if (!player.dead && CheckCollisionCirclesC(asteroids[i].collisionBody, player.collisionBody))
		{
			// kill player
			player.dead = 1;
     	death.timer.started = 1;
			death.timer.startTime = GetTime();
			death.destRect.x = player.position.x;
			death.destRect.y = player.position.y;
			PlaySound(deathSound);
			gameoverTimer.started = 1;
			gameoverTimer.startTime = death.timer.startTime;
		}
	}
}

void Gameover(void)
{
	if (GuiButton(retryButton.rect, retryButton.text))
	{
		player.dead = 0;
		death.timer.started = 0;
		gameoverTimer.started = 0;
		initializeGameboard();
	}

	quitToMainMenu = GuiButton(quitButton.rect, quitButton.text);
}
