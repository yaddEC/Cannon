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
	timeAlive += Time::GetDeltaTime();

	if (norme(this->position - initialPosition) < canonHeight / 13 && !isOut)
	{
		vitesseFinal = renderer.CannonExit(canonDeceleration, norme(this->position - initialPosition), canonInitalSpeed);

		this->position.x += vitesseFinal * cos(degToRad(canonAngle)) * Time::GetDeltaTime();
		this->position.y += vitesseFinal * sin(degToRad(canonAngle)) * Time::GetDeltaTime();
	}
	else
	{
		isOut = true;
		if (frictionState == ProjectileFriction::Quadratic)
			AccelerationWithQuadraticFriction(this);
		else if (frictionState == ProjectileFriction::Linear)
			AccelerationWithLinearFriction(this);
		else
			AccelerationWithoutFriction(this);

		speed += acceleration * Time::GetDeltaTime();

		float2 pos = position;
		printf("speed.x : %f\n", speed.x);
		printf("speed.y : %f\n", speed.y);
		if (pos.y < 0 + radius / 28 && !bounce)
		{
			bounce = true;
			time = 0;
			speed.y *= -0.75f;

		}

		if (bounceDelay > 0)
		{
			bounceDelay = 0;
			bounce = false;
		}
		if (bounce)
		{
			bounceDelay++;
		}

		position += speed * Time::GetDeltaTime();
	}


	renderer.dl->AddCircle(renderer.ToPixels(position), radius, IM_COL32_WHITE,0,4);

	DrawCurve(renderer);
}

void Sphere::DrawCurve(CannonRenderer& renderer)
{
	float2 point1 = initialPosition;
	float2 point2;
	for (int i = 0; i < 100; i++)
	{
		point2.x = i + initialPosition.x;

		if (i <= canonHeight / 13 * cos(degToRad(angle)))
		{
			point2.y = tan(degToRad(angle)) * (point2.x - initialPosition.x) + initialPosition.y;
		}
		else
		{
			newInitialPosition.x = initialPosition.x + canonHeight / 13 * cos(degToRad(angle));
			newInitialPosition.y = initialPosition.y + canonHeight / 13 * sin(degToRad(angle));
			newInitialSpeed = renderer.CannonExit(canonDeceleration, canonHeight, canonInitalSpeed);

			point2.y = -GRAVITY / 2 * pow((point2.x - newInitialPosition.x) / (newInitialSpeed * cos(degToRad(angle))), 2) + tan(degToRad(angle)) * (point2.x - newInitialPosition.x) + newInitialPosition.y;
		}

		renderer.dl->AddLine(renderer.ToPixels(point1), renderer.ToPixels(point2), IM_COL32_WHITE, 2);
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
