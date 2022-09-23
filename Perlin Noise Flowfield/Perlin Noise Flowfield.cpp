#include "raylib.h"
#include "PerlinNoise.h"
#include "Functions.h"
#include <vector>
#include <array>


std::array<std::array<double, 720>, 1280> flowfield;

int main()
{
    InitWindow(1280, 720, "Flowfield");
	SetTargetFPS(60);

	// Perlin noise setup
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	while (!WindowShouldClose())
	{
		// Update
		{

			for (int x = 0; x < 1280; x++)
			{
				for (int y = 0; y < 720; y++)
				{
					flowfield[x][y] = (Map(perlin.octave2D_01((x * 0.01), (y * 0.01), 4), 0, 1, 0, 255));
				}
			}
			
		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);

			for (int x = 0; x < 1280; x++)
			{
				for (int y = 0; y < 720; y++)
				{
					DrawPixel(x, y, Color(flowfield[x][y], 255, 255, 255));
				}
			}
		}
		EndDrawing();
	}
	CloseWindow();
}