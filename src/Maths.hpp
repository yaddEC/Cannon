#pragma once
#include "calc.hpp"
#include <cmath>

class Sphere;

float degToRad(float deg);
float radToDeg(float rad);
float norme(float2 vec);
float2 rotateVec(float2 tor, float2 origin, float angle);
void AccelerationWithoutFriction(Sphere* projectile);
void AccelerationWithLinearFriction(Sphere* projectile);
void AccelerationWithQuadraticFriction(Sphere* projectile);