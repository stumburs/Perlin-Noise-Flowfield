#include "raylib.h"
#include "raymath.h"
#include "PerlinNoise.h"
#include "Functions.h"
#include "Particle.h"
#include <vector>
#include <iostream>

const int WIDTH = 1280;
const int HEIGHT = 720;
const int SCALE = 40;

const int RENDER_WIDTH = WIDTH / SCALE + 1;
const int RENDER_HEIGHT = HEIGHT / SCALE + 1;

std::vector<std::vector<Vector2>> flowfield;

float flowfield_strength = 0.01f;

const size_t particle_count = 5000;
float particle_speed = 1.0f;
std::array<Particle, particle_count> particles;


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


	// Init particles
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].pos = { (float)GetRandomValue(0, WIDTH), (float)GetRandomValue(0, HEIGHT) };
	}

	// Init flowfield
	for (int x = 0; x < RENDER_WIDTH; x++)
	{
		flowfield.push_back(std::vector<Vector2>());
		for (int y = 0; y < RENDER_HEIGHT; y++)
		{
			// Calculate each vector
			double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (noise_height * z_mult), noise_detail), 0, 1, 0, 359);
			Vector2 vec = Vec2FromAngle(angle);
			vec = SetMagnitude(vec, 1);
			flowfield[x].push_back(vec);
		}
	}

	bool background_cleared = false;


	while (!WindowShouldClose())
	{
		// Update
		{
			// Flowfield
			for (int x = 0; x < flowfield.size(); x++)
			{
				for (int y = 0; y < flowfield[x].size(); y++)
				{
					// Calculate each vector
					double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (noise_height * z_mult), noise_detail), 0, 1, 0, 359);
					Vector2 vec = Vec2FromAngle(angle);
					vec = SetMagnitude(vec, flowfield_strength);
					flowfield[x][y] = vec;
				}

			}	

			// Update particles
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i].Update(flowfield, particle_speed, WIDTH, HEIGHT, SCALE);
			}

			// Move through noise
			noise_height += 0.02;
		}

		// Draw
		BeginDrawing();
		{
			//ClearBackground(BLACK);
			if (!background_cleared)
			{
				ClearBackground(BLACK);
				background_cleared = true;
			}

			//for (int x = 0; x < RENDER_WIDTH; x++)
			//{
			//	for (int y = 0; y < RENDER_HEIGHT; y++)
			//	{
			//		//DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, Color(flowfield[x][y], 0, 0, 255));
			//		DrawLine(	x * SCALE,
			//					y * SCALE,
			//					x * SCALE + Vector2Normalize(flowfield[x][y]).x * SCALE,
			//					y * SCALE + Vector2Normalize(flowfield[x][y]).y * SCALE,
			//					{255, 255, 255, 160});
			//		DrawCircle(x * SCALE, y * SCALE, 3, BLACK);
			//	}
			//}

			// Draw Particles
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i].Draw(1, {(unsigned char)Map(particles[i].pos.x, 0, WIDTH, 0, 255), 255, 255, 5});
			}
		}
		EndDrawing();
	}
	CloseWindow();
}