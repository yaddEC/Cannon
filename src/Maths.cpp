#include "Maths.hpp"
#include "Projectile.hpp"
#include "Time.h"

#define M_PI       3.14159265358979323846

float degToRad(float deg)
{
    return deg * M_PI / 180;
}

float radToDeg(float rad)
{
    return rad * 180 / M_PI;
}

float2 rotateVec(float2 tor, float2 origin, float angle)
{
    float2 tor2;

    tor2.x = cos(degToRad(angle)) * (tor.x - origin.x) - sin(degToRad(angle)) * (tor.y - origin.y) + origin.x;
    tor2.y = sin(degToRad(angle)) * (tor.x - origin.x) + cos(degToRad(angle)) * (tor.y - origin.y) + origin.y;

    return tor2;
}

void calculateSpeedWithoutFriction(Sphere* projectile)
{
    projectile->speed += projectile->acceleration * Time::GetDeltaTime();
}

void calculateSpeedWithLinearFriction(Sphere* projectile)
{
    float tau = projectile->mass / (6 * M_PI * VISCOSITY * projectile->radius);
    projectile->speed.y = (projectile->initialSpeed * sin(degToRad(projectile->angle)) + GRAVITY * tau) * exp(-1 * projectile->time / tau) - GRAVITY * tau;
    projectile->speed.x = projectile->initialSpeed * cos(degToRad(projectile->angle)) * exp(-1 * projectile->time / tau);
}