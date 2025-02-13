#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SPEED 400.0f

typedef struct Player
{
	TexturePro sprite;
	Circle collisionBody;
	Vector2 position;
	Vector2 direction;
	Vector2 velocity;
	float speed;
	float rotation;
} Player;

float GetPlayerSpeed(float curSpeed);
void UpdatePlayerPosition(Player* player, float dt);

#endif // !PLAYER_H
