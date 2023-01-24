#pragma once
#include "calc.hpp"
#include <cmath>

class Sphere;

float degToRad(float deg);
float radToDeg(float rad);
float2 rotateVec(float2 tor, float2 origin, float angle);
void calculateSpeedWithoutFriction(Sphere* projectile);
void calculateSpeedWithLinearFriction(Sphere* projectile);