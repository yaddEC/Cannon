#include "Projectile.hpp"

Sphere::Sphere(float radius)
{
	this->radius = radius;
}

void Sphere::Update(ImDrawList* dl)
{
	dl->AddCircle(pos, radius, IM_COL32_WHITE);
}