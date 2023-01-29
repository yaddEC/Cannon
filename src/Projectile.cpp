#include "Time.h"
#include "Maths.hpp"
#include "Projectile.hpp"
#include "cannon.hpp"
#include <string>

Sphere::Sphere(float radius, float initialS, float ang, float2 initialPos)
{
	this->radius = radius;
	initialSpeed = initialS;
	initialPosition = initialPos;
	angle = ang;
}

void Sphere::Update(CannonRenderer& renderer, int& score)
{
	time += Time::GetDeltaTime();
	timeAlive += Time::GetDeltaTime();

	float2 cannon = { initialPosition.x - canonWidth, initialPosition.y - canonHeight * 2 };
	float2 cannon2 = { initialPosition.x - canonWidth, initialPosition.y };
	float2 cannon3 = { initialPosition.x + canonWidth, initialPosition.y - canonHeight * 2 };
	
	if (norme(position - initialPosition) < norme(cannon - cannon2) && !isOut)
	{
		vitesseFinal = CannonFriction(canonDeceleration, norme(position - initialPosition), canonInitalSpeed);

		speed.x = vitesseFinal * cos(degToRad(canonAngle));
		speed.y = vitesseFinal * sin(degToRad(canonAngle));

		this->position.x += speed.x * Time::GetDeltaTime();
		this->position.y += speed.y * Time::GetDeltaTime();
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
	
		if (pos.y-radius < 0   && !bounce)
		{
			bounce = true;
			hasBounce = true;
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

			if (!firstBounce)
			{
				firstBounce = true;
			}
		}

		position += speed * Time::GetDeltaTime();
	}

	if (!firstBounce)
	{
		timeBeforeFirstBounce += Time::GetDeltaTime();
		lengthBeforeFirstBounce = position.x - initialPosition.x;
	}
	if (renderer.circleCircleCollision(position.x, position.y, norme(cannon - cannon3) / 2, 17.5, 8.5, 1) && !hasBounce && !scoreOnce)
	{
		score++;
		scoreOnce = true;
	}

	if (position.y > maxHeightReached)
		maxHeightReached = position.y;

	renderer.dl->AddCircle(renderer.ToPixels(position), norme(renderer.ToPixels(cannon) - renderer.ToPixels(cannon3)) / 2, IM_COL32_WHITE, 0, 4);

	DrawCurve(renderer);
}

void Sphere::DrawCurve(CannonRenderer& renderer)
{
	float2 point1 = initialPosition;
	float2 point2;

	float2 cannon = { initialPosition.x - canonWidth, initialPosition.y - canonHeight * 2 };
	float2 cannon2 = { initialPosition.x - canonWidth, initialPosition.y };

	for (int i = 0; i < 100; i++)
	{
		point2.x = i + initialPosition.x;

		if (i <= norme(cannon - cannon2) * cos(degToRad(angle)))
		{
			point2.y = tan(degToRad(angle)) * (point2.x - initialPosition.x) + initialPosition.y;
		}
		else
		{
			newInitialPosition.x = initialPosition.x + norme(cannon - cannon2) * cos(degToRad(angle));
			newInitialPosition.y = initialPosition.y + norme(cannon - cannon2) * sin(degToRad(angle));
			newInitialSpeed = CannonFriction(canonDeceleration, norme(newInitialPosition - initialPosition), canonInitalSpeed);

			point2.y = -GRAVITY / 2 * pow((point2.x - newInitialPosition.x) / (newInitialSpeed * cos(degToRad(angle))), 2) + (point2.x - newInitialPosition.x) * tan(degToRad(angle)) + newInitialPosition.y;
		}

		renderer.dl->AddLine(renderer.ToPixels(point1), renderer.ToPixels(point2), IM_COL32_WHITE, 2);
		point1 = point2;
	}

	float voy = newInitialSpeed * sin(degToRad(angle));
	float vox = newInitialSpeed * cos(degToRad(angle));
	float xo = initialPosition.x + canonHeight * cos(degToRad(angle));
	float yo = initialPosition.y + canonHeight * sin(degToRad(angle));
	float totalTime = (voy + sqrt(pow(voy, 2) + 2 * GRAVITY * yo)) / GRAVITY;
	float totalLength = vox * totalTime;
	float maxHeight = -voy / (2 * GRAVITY) + pow(voy, 2) / GRAVITY + yo;

	float2 pos1;
	pos1.x = xo - 1;
	pos1.y = yo + 3;
	std::string titles[8] = { 
		"Maximum height = " + std::to_string(maxHeight), 
		"Travel length = " + std::to_string(totalLength),
		"Travel time = " + std::to_string(totalTime),
		"Estimated projectile data :",
		"Maximun height reached = " + std::to_string(maxHeightReached),
		"Travel length = " + std::to_string(lengthBeforeFirstBounce),
		"Travel time = " + std::to_string(timeBeforeFirstBounce),
		"Current projectile data :"
	};

	for (int i = 0; i < 8; i++)
	{
		renderer.dl->AddText(renderer.ToPixels(pos1), IM_COL32_WHITE, titles[i].c_str());
		pos1.y += 0.5f;
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
