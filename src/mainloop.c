// raygui config
#define RAYGUI_IMPLEMENTATION
#ifdef DEBUG
#define RAYGUI_DEBUG_RECS_BOUNDS
#define RAYGUI_DEBUG_TEXT_BOUNDS
#endif // DEBUG

#include <time.h>
#include "raylib.h"
#include "raygui.h"
#include "mainloop.h"
#include "game.h"
#include "stars.h"

starCount = 1000;

void MainLoop(void)
{
	SetRandomSeed(time(NULL));
	boolean isGameStarting = 0;

  /***   initialize background assets   ***/
	// initialize earth
	earth.sprite = LoadTexture("Earth-Like planet.png");
	earth.timer.duration = 1.0 / 12.0;
	earth.srcRect = (Rectangle){ 0, 0, 96, 96 };
	earth.destRect = (Rectangle){ 1700, 300, 384, 384 };
	earth.origin = (Vector2){ 192, 192 };

	// initialize stars
	stars = LoadStars();
	starSprite = LoadTexture("star.png");

	// initialize bgm
#ifdef NDEBUG
	Music bgm = LoadMusicStream("Things From Space 1.mp3");
	PlayMusicStream(bgm);
	SetMusicVolume(bgm, 1.0f);
#endif // NDEBUG

	/***   create buttons   ***/
	Button startButton = { { 860, 530, 200, 20}, "Start New Game" };

	/***   main menu loop   ***/
	while (!WindowShouldClose())
	{

		/***   updates   ***/
    // deltaT
		float dt = GetFrameTime();

		// show cursor
		// when returning from gameloop
		if (IsCursorOnScreen())
			ShowCursor();

		// update planet animation
		if (CheckAnimationTimer(&earth.timer))
			UpdateAnimationSS(&earth);

   	// update bgm
#ifdef NDEBUG
		UpdateMusicStream(bgm);
#endif // NDEBUG
		
		/***   draw   ***/
   	BeginDrawing();
		ClearBackground(SPACE);

		// menu buttons
	
		// background
		DrawStars(stars, starSprite);
		DrawTexturePro(earth.sprite, earth.srcRect, earth.destRect, earth.origin, 0, RAYWHITE);

		isGameStarting = GuiButton(startButton.rect, startButton.text);

		EndDrawing();

		/***   initiate game   ***/
		// in the event of a window close 
		// we can go straight to cleanup
		if (isGameStarting)
		{
#ifdef NDEBUG
			StopMusicStream(bgm);
#endif // NDEBUG

			RunGame();

#ifdef NDEBUG
			PlayMusicStream(bgm);
#endif // NDEBUG
		}
	} // end main menu loop

	/***   cleanup   ***/
	UnloadTexture(earth.sprite);
	MemFree(stars);
#ifdef NDEBUG
	StopMusicStream(bgm);
	UnloadMusicStream(bgm);
#endif // NDEBUG
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

void LoadSpriteGroup(SpriteGroup* spriteGroup, float sizeFactor, char* dir, char* ft)
{
	spriteGroup->spriteArr = MemAlloc(spriteGroup->frameTotal * sizeof(Image));
	int bufCount = strlen(dir) + strlen(ft) + 6;
	char* filename = MemAlloc(bufCount * sizeof(char));
	for (int i = 0; i < spriteGroup->frameTotal; i++)
	{
		sprintf_s(filename, bufCount, "%s/%d.%s", dir, (i + 1), ft);
		spriteGroup->spriteArr[i] = LoadImage(filename);
	}
	MemFree(filename);
	spriteGroup->curSprite = LoadTextureFromImage(spriteGroup->spriteArr[0]);
	spriteGroup->frameIndex = 1;
	spriteGroup->srcRect = (Rectangle){ 0, 0, spriteGroup->curSprite.width, spriteGroup->curSprite.height };
	spriteGroup->destRect = (Rectangle){ 0, 0, sizeFactor * spriteGroup->curSprite.width, sizeFactor * spriteGroup->curSprite.height };
	spriteGroup->origin = (Vector2){ sizeFactor * spriteGroup->curSprite.width / 2, sizeFactor * spriteGroup->curSprite.height / 2 };
}

void UpdateAnimationSG(SpriteGroup* spriteGroup)
{
	if (spriteGroup->frameIndex < spriteGroup->frameTotal)
		spriteGroup->curSprite = LoadTextureFromImage(spriteGroup->spriteArr[spriteGroup->frameIndex++]);
	else if (spriteGroup->frameIndex == spriteGroup->frameTotal)
		spriteGroup->frameIndex++;
}

void DrawAnimationOnceSG(SpriteGroup spriteGroup)
{
	if (spriteGroup.timer.started && spriteGroup.frameIndex < (spriteGroup.frameTotal + 1))
		DrawTexturePro(spriteGroup.curSprite, spriteGroup.srcRect, spriteGroup.destRect, spriteGroup.origin, 0, RAYWHITE);
}

void UpdateAnimationSS(SpriteSheet* spriteSheet)
{
		if (spriteSheet->srcRect.x >= spriteSheet->sprite.width - spriteSheet->srcRect.width)
			spriteSheet->srcRect.x = 0;
		else
			spriteSheet->srcRect.x += spriteSheet->srcRect.width;
}

