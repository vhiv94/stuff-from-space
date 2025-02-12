/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "asteroids.h"
#include <time.h>

#define MAX_SPEED 400.0f

int main ()
{
	int asteroidCount = 100;
	char fps[9] = {'\0'};

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1920, 1080, "Asteroids");
	SetExitKey(KEY_ESCAPE);

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
	};
	Texture asteroidTxtr = LoadTexture("Asteroid 01 - Base.png");
	Entity* asteroids = LoadAsteroids(asteroidCount);

	Camera2D camera =
	{
		.offset = { 960, 540 },
		.zoom = 1.0f,
		.target = player.position,
	};

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// inputs
		if (IsCursorOnScreen())
			HideCursor();
#if 0
		shipPosition = GetMousePosition();
		if (shipPosition.x > 584)
			shipPosition.x = 584;
		if (shipPosition.y > 682)
			shipPosition.y = 682;

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			// shoot()
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			// missile()

		player.direction.x = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
		player.direction.y = (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) - (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W));
		player.direction = Vector2Normalize(player.direction);
#endif
		player.speed = GetPlayerSpeed(player.speed);
		player.rotation += (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT));
		player.direction = GetPlayerDirection(player.rotation);


		// updates 
		// deltaT
		float dt = GetFrameTime();

		// wrap player
		if (player.position.x <= -4000 || player.position.x >= 4000)
			player.position.x *= -1;
		if (player.position.y <= -2000 || player.position.y >= 2000)
			player.position.y *= -1;

		// player position
		player.position.x += player.direction.x * player.speed * dt;
		player.position.y += player.direction.y * player.speed * dt;
		player.sprite.destRect.x = player.position.x;
		player.sprite.destRect.y = player.position.y;
		player.collisionBody.center = (Vector2){  player.position.x, player.position.y };

		camera.target = player.position;
		if (player.speed > 100)
			camera.zoom = 1.0f - ((player.speed - 100.0f) / (5.0f * MAX_SPEED));
		printf("%f, %f\n", player.speed, camera.zoom);

		UpdateAsteroidPositions(asteroids, asteroidCount, dt);

		// collision
		//CheckCollisionBoxSphere();
		//bool isColliding =  CheckCollisionCircles(player.collisionBody.center, player.collisionBody.radius, (Vector2) { 300, 300 }, 30.0f);

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
		DrawTexturePro(player.sprite.spriteImg, player.sprite.srcRect, player.sprite.destRect, (Vector2) { 28, 22 }, player.rotation, RAYWHITE);
		DrawAsteroids(asteroids, asteroidCount, asteroidTxtr);

#ifdef DEBUG
		DrawCircleV(player.collisionBody.center, player.collisionBody.radius, TRANSLUCENT);
#endif // DEBUG

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndMode2D();
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(player.sprite.spriteImg);
	UnloadTexture(asteroidTxtr);
	MemFree(asteroids);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

float GetPlayerSpeed(float curSpeed)
{
	if (curSpeed <= MAX_SPEED)
		curSpeed += (float)((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) - (IsKeyUp(KEY_W) && IsKeyUp(KEY_UP)));
	if (curSpeed > MAX_SPEED)
		return MAX_SPEED;
	else if (curSpeed < 0)
		return 0.0f;
	return curSpeed;
}

Vector2 GetPlayerDirection(float rotation)
{
	float radian = DEG2RAD * rotation;
	Vector2 direction = { sinf(radian), -cosf(radian) };
	direction = Vector2Normalize(direction);
	return direction;
}

Vector2 GetPlayerVelocity(Vector2 direction, float speed)
{
	Vector2 velocity;
	velocity.x = direction.x * speed;
	velocity.y = direction.y * speed;
}

Entity* LoadAsteroids(int count)
{
	Entity* asteroids = (Entity*)MemAlloc(count * sizeof(Entity));
	for (int i = 0; i < count; i++)
	{
		asteroids[i].position = (Vector2){ GetRandomValue(-4000, 4000), GetRandomValue(-2000, 2000) };
		asteroids[i].direction = (Vector2){ GetRandomValue(-1, 1), GetRandomValue(-1, 1) };
		asteroids[i].direction = Vector2Normalize(asteroids[i].direction);
		asteroids[i].speed = (float)GetRandomValue(20, 100);
		asteroids[i].collisionBody.radius = 8 * GetRandomValue(1, 3);
		asteroids[i].collisionBody.center.x = asteroids[i].position.x + (1.125f * asteroids[i].collisionBody.radius);
		asteroids[i].collisionBody.center.y = asteroids[i].position.y + asteroids[i].collisionBody.radius;
	}
	return asteroids;
}

void UpdateAsteroidPositions(Entity* asteroids, int count, float dt)
{
	for (int i = 0; i < count; i++)
	{
		if (asteroids[i].position.x <= -4000 || asteroids[i].position.x >= 4000)
			asteroids[i].position.x *= -1;
		if (asteroids[i].position.y <= -2000 || asteroids[i].position.y >= 2000)
			asteroids[i].position.y *= -1;
		asteroids[i].position.x += asteroids[i].direction.x * asteroids[i].speed * dt;
		asteroids[i].position.y += asteroids[i].direction.y * asteroids[i].speed * dt;
		asteroids[i].collisionBody.center.x = asteroids[i].position.x + (1.125f * asteroids[i].collisionBody.radius);
		asteroids[i].collisionBody.center.y = asteroids[i].position.y + asteroids[i].collisionBody.radius;

	}
}

void DrawAsteroids(Entity* asteroids, int count, Texture texture)
{
	for (int i = 0; i < count; i++)
	{
		DrawTextureEx(texture, asteroids[i].position, 0, asteroids[i].collisionBody.radius / 16.0f, RAYWHITE);
#ifdef DEBUG
		DrawCircleV(asteroids[i].collisionBody.center, asteroids[i].collisionBody.radius, TRANSLUCENT);
#endif // DEBUG

	}
}