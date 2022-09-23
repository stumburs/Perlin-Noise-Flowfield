#pragma once
#include "raylib.h"
#include "raymath.h"

struct Particle
{
    Vector2 pos; // Position
    Vector2 vel; // Velocity
    Vector2 acc; // Acceleration

    void Update(float max_speed, int WIDTH, int HEIGHT);

    void ApplyForce(Vector2 force);

    void Follow(Vector2 flowfield[], int SCALE);

    void WrapAroundEdges(int WIDTH, int HEIGHT);

    void Draw(float size, Color color);
};