#pragma once
#include "calc.hpp"
#include <imgui.h>

class CannonRenderer;

enum class ProjectileFriction
{
	Linear,
	Quadratic,
	None,
};

 class Projectile 
{

public:
	float mass;
	float dragForce;
	float contactArea;
	float initialSpeed;
	float angle;
	float time = 0;

	ProjectileFriction frictionState = ProjectileFriction::Quadratic;

	float2 direction;
	float2 position;
	float2 initialPosition;
	float2 speed;
	float2 acceleration;

	Projectile() {};
	~Projectile() {};

	virtual void Update(CannonRenderer& renderer) {};
	virtual void DrawCurve(CannonRenderer& renderer) {};
	virtual void Init() {};

};

 class Sphere: public Projectile
 {
 public:
	 float radius;

	 Sphere() {};
	 Sphere(float radius, float initialS, float ang, float2 initialPos);
	 ~Sphere() {};

	 void Update(CannonRenderer& renderer) override;
	 void DrawCurve(CannonRenderer& renderer) override;
	 void Init() override;
 };

