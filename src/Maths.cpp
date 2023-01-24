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

void AccelerationWithoutFriction(Sphere* projectile)
{
    projectile->acceleration.x = 0;
    projectile->acceleration.y = -GRAVITY;
}

void AccelerationWithLinearFriction(Sphere* projectile)
{
    float k = 6 * M_PI * VISCOSITY * projectile->radius/20;
    projectile->acceleration.x = (-1 * k * projectile->speed.x) / projectile->mass;
    projectile->acceleration.y = (-1 * k * projectile->speed.y - projectile->mass * GRAVITY) / projectile->mass;
}

void AccelerationWithQuadraticFriction(Sphere* projectile)
{
    float Rho = 1.225f;
    float surface = pow(projectile->radius/20, 2) * 2 * M_PI;
    float Cx = 0.45f;
    float k = 0.5f * Rho * surface * Cx;

    float speed = sqrt(pow(projectile->speed.x, 2) + pow(projectile->speed.y, 2));

    projectile->acceleration.x = (-1 * k * speed * projectile->speed.x) / projectile->mass;
    projectile->acceleration.y = (-1 * k * speed * projectile->speed.y - projectile->mass * GRAVITY) / projectile->mass;
}