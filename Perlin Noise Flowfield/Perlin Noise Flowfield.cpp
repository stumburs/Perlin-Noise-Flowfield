#include "raylib.h"
#include "raymath.h"
#include "PerlinNoise.h"
#include "Functions.h"
#include <vector>
#include <array>

const int WIDTH = 1280;
const int HEIGHT = 720;
const int SCALE = 40;

const int RENDER_WIDTH = WIDTH / SCALE + 1;
const int RENDER_HEIGHT = HEIGHT / SCALE + 1;

std::array<std::array<Vector2, RENDER_HEIGHT>, RENDER_WIDTH> flowfield;

double noise_height = 0;
double noise_detail = 6;
double x_mult = 0.02;
double y_mult = 0.02;
double z_mult = 0.02;

int main()
{
    InitWindow(1280, 720, "Flowfield");
	SetConfigFlags(FLAG_MSAA_4X_HINT);
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
					// Calculate each vector
					double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (noise_height * z_mult), noise_detail), 0, 1, 0, 359);
					Vector2 vec = Vec2FromAngle(angle);
					vec = SetMagnitude(vec, 1);
					flowfield[x][y] = vec;
				}
			}	

			noise_height += 0.005;
		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);

			for (int x = 0; x < RENDER_WIDTH; x++)
			{
				for (int y = 0; y < RENDER_HEIGHT; y++)
				{
					//DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, Color(flowfield[x][y], 0, 0, 255));
					DrawLine(	x * SCALE,
								y * SCALE,
								x * SCALE + Vector2Normalize(flowfield[x][y]).x * SCALE,
								y * SCALE + Vector2Normalize(flowfield[x][y]).y * SCALE,
								{255, 255, 255, 150});
					DrawCircle(x * SCALE, y * SCALE, 3, BLACK);
				}
			}
		}
		EndDrawing();
	}
	CloseWindow();
}