#include "raylib.h"
#include "raymath.h"
#include "sfs.h"
#include "player.h"

void GetPlayerSpeed(float* speed)
{
	if (*speed <= MAX_SPEED)
		*speed += (float)((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) - (IsKeyUp(KEY_W) && IsKeyUp(KEY_UP)));
	if (*speed > MAX_SPEED)
		*speed = MAX_SPEED;
	else if (*speed < 0)
		*speed = 0.0f;
}

void getPlayerInput(Player* player)
{
 	GetPlayerSpeed(&player->speed);
	player->rotation += (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT));
	player->direction = GetNormalizedDirection(player->rotation);
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
