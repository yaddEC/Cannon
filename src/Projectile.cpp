#include "Time.h"
#include "Maths.hpp"
#include "Projectile.hpp"
#include "cannon.hpp"

Sphere::Sphere(float radius)
{
	this->radius = radius;
}

void Sphere::Update(CannonRenderer& renderer)
{
	time += Time::GetDeltaTime();
	pos.x = this->initialSpeed * cos(degToRad(this->angle)) * time + initialPos.x;
	pos.y += (-GRAVITY / 2) * pow(this->time, 2) + this->initialSpeed * sin(degToRad(this->angle)) * time + initialPos.y;
	renderer.dl->AddCircle(renderer.ToPixels(pos), radius, IM_COL32_WHITE);


	float2 point1 = initialPos;
	float2 point2;
	for (int i = 0; i < 100; i++)
	{
		point2.x = i + initialPos.x;
		point2.y = -GRAVITY / 2 * pow((point2.x - initialPos.x) / (initialSpeed * cos(degToRad(angle))), 2) + tan(degToRad(angle)) * (point2.x - initialPos.x) + initialPos.y;
		renderer.dl->AddLine(renderer.ToPixels(point1), renderer.ToPixels(point2), IM_COL32_WHITE);
		point1 = point2;
	}
}