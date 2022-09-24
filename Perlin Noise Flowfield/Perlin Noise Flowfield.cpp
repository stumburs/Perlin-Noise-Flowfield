#include "raylib.h"
#include "raymath.h"
#include "PerlinNoise.h"
#include "Functions.h"
#include "Particle.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

const int WIDTH = 1920;
const int HEIGHT = 1080;
int scale = 20;

std::vector<std::vector<Vector2>> flowfield;

float flowfield_strength = 0.01f;
size_t particle_count = 10000;
float particle_speed = 1.0f;
float particle_size = 1.0f;
unsigned char particle_strength = 1;
std::vector<Particle> particles;

double noise_height = 0;
int32_t noise_detail = 6;
double x_mult = 0.02;
double y_mult = 0.02;
double z_mult = 0.02;

int main()
{

	// Read Preset
	if (FileExists("preset.ff"))
	{
		std::ifstream preset_file("preset.ff");
		std::string line;
		double values[13]{};
		int value = 0;

		// Read file
		if (preset_file.is_open())
		{
			while (std::getline(preset_file, line))
			{
				values[value++] = std::stod(line.substr(line.find(' ') + 1));
			}
			preset_file.close();
		}

		// Apply preset
		// WIDTH = [0]
		// HEIGHT = [1]
		scale = (int)values[2];
		flowfield_strength = (float)values[3];
		particle_count = (size_t)values[4];
		particle_speed = (float)values[5];
		particle_size = (float)values[6];
		particle_strength = (unsigned char)values[7];
		noise_height = (double)values[8];
		noise_detail = (int32_t)values[9];
		x_mult = (double)values[10];
		y_mult = (double)values[11];
		z_mult = (double)values[12];

	}

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE);
    InitWindow(WIDTH, HEIGHT, "Flowfield");
	SetTargetFPS(60);

	int render_width = WIDTH / scale + 1;
	int render_height = HEIGHT / scale + 1;

	// Perlin noise setup
	const siv::PerlinNoise::seed_type seed = 69420u;
	const siv::PerlinNoise perlin{ seed };

	// Init particles
	for (int i = 0; i < particle_count; i++)
	{
		Particle p;
		p.pos = { (float)GetRandomValue(0, WIDTH), (float)GetRandomValue(0, HEIGHT) };
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
				particles[i].Update(flowfield, particle_speed, WIDTH, HEIGHT, scale);
			}

			// Move through noise
			noise_height += 0.06;

			// Take screenshot
			//if (IsKeyPressed(KEY_ENTER)) TakeScreenshot("image.png");
			if (IsKeyPressed(KEY_ENTER))
			{
				Image screenshot= LoadImageFromScreen();
				ExportImage(screenshot, "screen.png");
			}

			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
			{
				std::ofstream preset_file;
				preset_file.open("preset.ff");
				if (preset_file.is_open())
				{
					preset_file <<	"WIDTH 1920\n" <<
									"HEIGHT 1080\n" <<
									"SCALE 20\n" <<
									"flowfield_strength 0.01f\n" <<
									"particle_count 10000\n" <<
									"particle_speed 1.0f\n" <<
									"particle_size 1.0f\n" <<
									"particle_strength 1\n" <<
									"noise_height 0\n" <<
									"noise_detail 6\n" <<
									"x_mult 0.02\n" <<
									"y_mult 0.02\n" <<
									"z_mult 0.02\n";
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
				/*particles[i].DrawCircle(	particle_size,
										{(unsigned char)Map(particles[i].pos.x, 0, WIDTH, 0, 255),
										255,
										(unsigned char)Map(particles[i].pos.y, 0, HEIGHT, 0, 255),
										particle_strength});*/
				particles[i].DrawPixel(		{ 255,
											(unsigned char)Map(particles[i].pos.x, 0, WIDTH, 0, 255),
											(unsigned char)Map(particles[i].pos.y, 0, HEIGHT, 0, 255),
											particle_strength });
			}
		}
		EndDrawing();
	}
	CloseWindow();
}