#include "Time.h"
#include "Maths.hpp"
#include "Projectile.hpp"
#include "cannon.hpp"

Sphere::Sphere(float radius, float initialS, float ang, float2 initialPos)
{
	this->radius = radius;
	initialSpeed = initialS;
	initialPosition = initialPos;
	angle = ang;
}

void Sphere::Update(CannonRenderer& renderer)
{
	time += Time::GetDeltaTime();
	
	/*
	// Donne la position directement via les equations horaires
	acceleration.x = 0;
	acceleration.y = -GRAVITY;

	speed.x = acceleration.x * time + initialSpeed * cos(degToRad(angle));
	speed.y = acceleration.y * time + initialSpeed * sin(degToRad(angle));

	position.x = (acceleration.x / 2) * pow(time, 2) + initialSpeed * cos(degToRad(angle)) * time + initialPosition.x;
	position.y = (acceleration.y / 2) * pow(time, 2) + initialSpeed * sin(degToRad(angle)) * time + initialPosition.y;
	*/

	//Donne la position par incrementation
	speed += acceleration * Time::GetDeltaTime();
	
	float2 pos = position;
	if (pos.y < 0)
		speed.y *= -0.75f;
		
	position += speed * Time::GetDeltaTime();


	renderer.dl->AddCircle(renderer.ToPixels(position), radius, IM_COL32_WHITE);

	DrawCurve(renderer);
}

void Sphere::DrawCurve(CannonRenderer& renderer)
{
	float2 point1 = initialPosition;
	float2 point2;
	for (int i = 0; i < 100; i++)
	{
		point2.x = i + initialPosition.x;
		point2.y = -GRAVITY / 2 * pow((point2.x - initialPosition.x) / (initialSpeed * cos(degToRad(angle))), 2) + tan(degToRad(angle)) * (point2.x - initialPosition.x) + initialPosition.y;
		renderer.dl->AddLine(renderer.ToPixels(point1), renderer.ToPixels(point2), IM_COL32_WHITE);
		point1 = point2;
	}
}

void Sphere::Init()
{
	time = 0;

	position = initialPosition;

	speed.x = initialSpeed * cos(degToRad(angle));
	speed.y = initialSpeed * sin(degToRad(angle));

	acceleration.x = 0;
	acceleration.y = -GRAVITY;
}
