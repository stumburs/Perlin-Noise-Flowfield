#include "Functions.h"

double Map(double n, double start1, double stop1, double start2, double stop2)
{
    return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}

Vector2 Vec2FromAngle(int angle)
{
    return Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
}

Vector2 SetMagnitude(Vector2 v, float magnitude)
{
    Vector2 newVec = Vector2Normalize(v);
    newVec.x = v.x * magnitude;
    newVec.y = v.y * magnitude;
    return newVec;
}