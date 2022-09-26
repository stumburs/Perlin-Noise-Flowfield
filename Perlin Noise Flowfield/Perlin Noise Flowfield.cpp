#include "raylib.h"
#include "raymath.h"
#include "PerlinNoise.h"
#include "Functions.h"
#include "Particle.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <variant>
#include "Preset.h"

const int window_width = 1920;
const int window_height = 1080;
int scale = 20;

std::vector<std::vector<Vector2>> flowfield;

float flowfield_strength = 0.01f;
int particle_count = 10000;
float particle_speed = 1.0f;
float particle_size = 1.0f;
unsigned char particle_strength = 1;
std::vector<Particle> particles;

float noise_height = 0;
int noise_detail = 6;
float x_mult = 0.02;
float y_mult = 0.02;
float z_mult = 0.02;
float z = 0;

Preset p_test;

int main()
{
	// Read Preset
	if (FileExists("preset.ff"))
	{
		p_test = ReadPreset("preset.ff");
		auto test = p_test.values.find("particle_speed");

		if (test == p_test.values.end())
		{
			std::cout << "not found";
		}
		else
		{
			std::cout << std::get<decltype(test->second)>(p_test.values.begin());

		}
	}

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE);
    InitWindow(window_width, window_height, "Flowfield");
	SetTargetFPS(60);

	int render_width = window_width / scale + 1;
	int render_height = window_height / scale + 1;

	// Perlin noise setup
	const siv::PerlinNoise::seed_type seed = 69420u;
	const siv::PerlinNoise perlin{ seed };

	// Init particles
	for (int i = 0; i < particle_count; i++)
	{
		Particle p;
		p.pos = { (float)GetRandomValue(0, window_width), (float)GetRandomValue(0, window_height) };
		particles.push_back(p);
	}

	// Init flowfield
	for (int x = 0; x < render_width; x++)
	{
		flowfield.push_back(std::vector<Vector2>());
		for (int y = 0; y < render_height; y++)
		{
			// Calculate each vector
			float angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (noise_height * z_mult), noise_detail), 0, 1, 0, 359);
			Vector2 vec = Vec2FromAngle(angle);
			vec = SetMagnitude(vec, 1);
			flowfield[x].push_back(vec);
		}
	}

	// Set noise height
	z = noise_height;

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
					double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (z * z_mult), noise_detail), 0, 1, 0, 359);
					Vector2 vec = Vec2FromAngle(angle);
					vec = SetMagnitude(vec, flowfield_strength);
					flowfield[x][y] = vec;
				}

			}	

			// Update particles
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i].Update(flowfield, particle_speed, window_width, window_height, scale);
			}

			// Move through noise
			z += 0.06;

			// Take screenshot
			if (IsKeyPressed(KEY_ENTER))
			{
				Image screenshot= LoadImageFromScreen();
				ExportImage(screenshot, "screen.png");
			}

			// Save preset
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
			{
				std::ofstream preset_file;
				preset_file.open("preset.ff");
				if (preset_file.is_open())
				{
					preset_file <<	"window_width " << window_width << "\n" <<
									"window_height " << window_height << "\n" <<
									"scale " << scale << "\n" <<
									"flowfield_strength " << flowfield_strength << "\n" <<
									"particle_count " << particle_count << "\n" <<
									"particle_speed " << particle_speed << "\n" <<
									"particle_size " << particle_size << "\n" <<
									"particle_strength " << (int)particle_strength << "\n" <<
									"noise_height " << noise_height << "\n" <<
									"noise_detail " << noise_detail << "\n" <<
									"x_mult " << x_mult << "\n" <<
									"y_mult " << y_mult << "\n" <<
									"z_mult " << z_mult;
					preset_file.close();
				}
			}
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

			//for (int x = 0; x < render_width; x++)
			//{
			//	for (int y = 0; y < render_height; y++)
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
				particles[i].DrawPixel(		{ 255,
											(unsigned char)Map(particles[i].pos.x, 0, window_width, 0, 255),
											(unsigned char)Map(particles[i].pos.y, 0, window_height, 0, 255),
											particle_strength });
			}
		}
		EndDrawing();
	}
	CloseWindow();
}