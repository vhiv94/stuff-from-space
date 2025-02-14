#include "raylib.h"
#include "raymath.h"
#include "sfs.h"
#include "player.h"

#include <stdio.h>

void GetPlayerInputs(Player* player, float dt)
{
	if (player->dead)
		return;
	// direction input
  player->rotation += (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT));
	player->direction = GetDirectionFromRotation(player->rotation);
	
	// deceleration
	if (IsKeyUp(KEY_W) && IsKeyUp(KEY_UP) && IsKeyUp(KEY_S) && IsKeyUp(KEY_DOWN))
	{
		if (player->speed == 0)
			return;
		Vector2 velocityDirection = Vector2Normalize(player->velocity);
		player->velocity.x -= velocityDirection.x * dt / 1.5f;
		player->velocity.y -= velocityDirection.y * dt / 1.5f;
		player->speed = sqrtf((player->velocity.x * player->velocity.x) + (player->velocity.y * player->velocity.y));
		if (player->speed < 0)
		{
			player->velocity = (Vector2){ 0,0 };
			player->speed = 0.0f;
		}
	}
	else
	{
		// velocity input
		float curAcceleration = ((player->acceleration * (float)(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))) - \
			(player->acceleration * (float)(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) / 2.0f));
		player->velocity.x += player->direction.x * dt * curAcceleration;
		player->velocity.y += player->direction.y * dt * curAcceleration;

		// cap velocity
		player->speed = sqrtf((player->velocity.x * player->velocity.x) + (player->velocity.y * player->velocity.y));
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
	// player position
	player->position = Vector2Add(player->position, player->velocity);

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
