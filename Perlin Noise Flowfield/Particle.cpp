#pragma once
#include "Particle.h"
#include "raylib.h"
#include "raymath.h"

void Particle::Update(float max_speed, int WIDTH, int HEIGHT)
{
    vel = Vector2Add(vel, acc);
    vel = Vector2Normalize(vel);
    pos = Vector2Add(pos, Vector2Scale(vel, max_speed));
    WrapAroundEdges(WIDTH, HEIGHT);
    acc = { 0 };
}

void Particle::ApplyForce(Vector2 force)
{
    acc = Vector2Add(acc, force);
}

void Particle::Follow(std::vector<std::vector<Vector2>> flowfield, int SCALE)
{
    float x = floor(pos.x / SCALE);
    float y = floor(pos.y / SCALE);
    Vector2 force = flowfield[x][y];
    ApplyForce(force);
}

void Particle::WrapAroundEdges(int WIDTH, int HEIGHT)
{
    if (pos.x > WIDTH && acc.x > 0) pos.x = 0;
    if (pos.x < 0 && acc.x < 0) pos.x = WIDTH;
    if (pos.y > HEIGHT && acc.y > 0) pos.y = 0;
    if (pos.y < 0 && acc.y < 0) pos.y = HEIGHT;
}

void Particle::Draw(float size, Color color)
{
    DrawCircle(pos.x, pos.y, size, color);
}
