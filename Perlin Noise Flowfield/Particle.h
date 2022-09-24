#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Particle
{
    Vector2 pos; // Position
    Vector2 vel; // Velocity
    Vector2 acc; // Acceleration

    void Update(std::vector<std::vector<Vector2>> flowfield, float max_speed, int WIDTH, int HEIGHT, int SCALE);

    void Follow(std::vector<std::vector<Vector2>> flowfield, int SCALE);
    
    void ApplyForce(Vector2 force);

    void WrapAroundEdges(int WIDTH, int HEIGHT);

    void Draw(float size, Color color);
};