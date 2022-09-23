#include "raylib.h"
#include "PerlinNoise.h"
#include "Functions.h"
#include <vector>
#include <array>

const int WIDTH = 1280;
const int HEIGHT = 720;
const int SCALE = 5;

const int RENDER_WIDTH = WIDTH / SCALE;
const int RENDER_HEIGHT = HEIGHT / SCALE;

std::array<std::array<double, RENDER_HEIGHT>, RENDER_WIDTH> flowfield;

double noise_height = 0;
double noise_detail = 6;
double x_mult = 0.02;
double y_mult = 0.02;

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
			for (int x = 0; x < RENDER_WIDTH; x++)
			{
				for (int y = 0; y < RENDER_HEIGHT; y++)
				{
					flowfield[x][y] = (Map(perlin.octave3D_01((x * x_mult), (y * y_mult), noise_height, noise_detail), 0, 1, 0, 255));
				}
			}	

			noise_height += 0.01;
		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);

			for (int x = 0; x < RENDER_WIDTH; x++)
			{
				for (int y = 0; y < RENDER_HEIGHT; y++)
				{
					DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, Color(flowfield[x][y], 0, 0, 255));
				}
			}
		}
		EndDrawing();
	}
	CloseWindow();
}