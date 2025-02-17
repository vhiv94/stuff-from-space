#include "raylib.h"
#include "raymath.h"
#include "mainloop.h"
#include "game.h"
#include "player.h"

#include <stdio.h>

static float GetSpeed(Vector2 velocity)
{
	return sqrtf((velocity.x * velocity.x) + (velocity.y * velocity.y));
}

void GetPlayerInputs(Player* player)
{
	if (player->dead)
		return;

	// rotation input
  player->rotation += (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT));
	float radian = DEG2RAD * player->rotation;
	player->direction = (Vector2){ sinf(radian), -cosf(radian) };

	// deceleration
	if (IsKeyUp(KEY_W) && IsKeyUp(KEY_UP) && IsKeyUp(KEY_S) && IsKeyUp(KEY_DOWN))
	{
		if (!player->speed)
			return;

		Vector2 velocityDirection = Vector2Normalize(player->velocity);
		player->velocity.x -= velocityDirection.x / 1.5f;
		player->velocity.y -= velocityDirection.y / 1.5f;

		player->speed = GetSpeed(player->velocity);
		if (player->speed <= 0.001f)
			player->speed = 0.0f;
		/*
		if (player->speed < 0)
			player->velocity = (Vector2){ 0,0 };
		*/
	}
	else
	{
		// velocity input
		float forwardInput = player->acceleration * (float)(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP));
		float backwardInput = player->acceleration * (float)(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) / 2.0f;
		float curAcceleration = forwardInput - backwardInput;
		player->velocity.x += player->direction.x * curAcceleration;
		player->velocity.y += player->direction.y * curAcceleration;

		// cap velocity
		player->speed = GetSpeed(player->velocity);
		if (player->speed > MAX_SPEED)
		{
			player->velocity = Vector2Normalize(player->velocity);
			player->velocity.x *= MAX_SPEED;
			player->velocity.y *= MAX_SPEED;
			player->speed = MAX_SPEED;
		}
		else if (player->speed < -MAX_SPEED)
		{
			player->velocity = Vector2Normalize(player->velocity);
			player->velocity.x *= -MAX_SPEED;
			player->velocity.y *= -MAX_SPEED;
			player->speed = -MAX_SPEED;
		}
	}
}

void UpdatePlayerPosition(Player* player, float dt)
{
	if (player->dead)
		return;

	// player position
	player->position.x += player->velocity.x * dt;
	player->position.y += player->velocity.y * dt;

	// wrap player
	if (player->position.x <= -4000 || player->position.x >= 4000)
		player->position.x *= -1;
	if (player->position.y <= -2000 || player->position.y >= 2000)
		player->position.y *= -1;

	// sprite position
	player->sprite.destRect.x = player->position.x;
	player->sprite.destRect.y = player->position.y;

	// collision body position
	player->collisionBody.center = player->position;
}

void DrawPlayer(Player player)
{
  DrawTexturePro(player.sprite.spriteImg, player.sprite.srcRect, player.sprite.destRect, player.sprite.origin, player.rotation, RAYWHITE);
#ifdef DEBUG
	if (!player.dead)
		DrawCircleV(player.collisionBody.center, player.collisionBody.radius, TRANSLUCENT);
#endif // DEBUG

}

void StartDeath(Player player, SpriteGroup* death, Sound sound)
{
	if (player.dead && !death->timer.started)
	{
		death->timer.started = 1;
		death->timer.startTime = GetTime();
		death->destRect.x = player.position.x;
		death->destRect.y = player.position.y;
		PlaySound(sound);
	}
}
