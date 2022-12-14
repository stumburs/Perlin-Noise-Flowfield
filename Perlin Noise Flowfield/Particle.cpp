#pragma once
#include "Particle.h"
#include "raylib.h"
#include "raymath.h"

void Particle::Update(const std::vector<std::vector<Vector2>> &flowfield, float max_speed, int WIDTH, int HEIGHT, int SCALE)
{
    vel = Vector2Add(vel, acc);
    vel = Vector2Normalize(vel);
    pos = Vector2Add(pos, Vector2Scale(vel, max_speed));
    WrapAroundEdges(WIDTH, HEIGHT);
    acc = { 0 };
    Follow(flowfield, SCALE);
}

void Particle::Follow(const std::vector<std::vector<Vector2>> &flowfield, int SCALE)
{
    float x = floorf(pos.x / SCALE);
    float y = floorf(pos.y / SCALE);
    Vector2 force = flowfield[x][y];
    ApplyForce(force);
}

void Particle::ApplyForce(Vector2 force)
{
    acc = Vector2Add(acc, force);
}

void Particle::WrapAroundEdges(int WIDTH, int HEIGHT)
{
    if (pos.x > (float)WIDTH) pos.x = 0;
    if (pos.x < 0) pos.x = (float)WIDTH;
    if (pos.y > (float)HEIGHT) pos.y = 0;
    if (pos.y < 0) pos.y = (float)HEIGHT;
}

void Particle::DrawCircle(float size, Color color)
{
    RAYLIB_H::DrawCircle(pos.x, pos.y, size, color);
}

void Particle::DrawPixel(Color color)
{
    RAYLIB_H::DrawPixel(pos.x, pos.y, color);
}
