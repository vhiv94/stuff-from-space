#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SPEED 400.0f

typedef struct
{
	Sprite sprite;
	Circle collisionBody;
	Vector2 position;
	Vector2 direction;
	Vector2 velocity;
	float speed;
	float acceleration;
	float rotation;
	boolean dead;
} Player;

void GetPlayerInputs(Player* player);
void UpdatePlayerPosition(Player* player, float dt);
void DrawPlayer(Player player);
void StartDeath(Player player, SpriteGroup* death, Sound sound);

#endif // !PLAYER_H
