#include "raylib.h"
#include "raymath.h"
#include "sfs.h"
#include "player.h"

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

Vector2 GetPlayerVelocity(Vector2 direction, float speed)
{
	Vector2 velocity;
	velocity.x = direction.x * speed;
	velocity.y = direction.y * speed;
}

void UpdatePlayerPosition(Player* player, float dt)
{
// player position
		player->position.x += player->direction.x * player->speed * dt;
		player->position.y += player->direction.y * player->speed * dt;

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
