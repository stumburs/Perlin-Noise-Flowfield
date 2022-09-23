#include <iostream>
#include "raylib.h"
#include "PerlinNoise.h"

int main()
{
    InitWindow(1280, 720, "Flowfield");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		// Update

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);
		}
		EndDrawing();
	}
	CloseWindow();
}