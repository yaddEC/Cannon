#pragma once
#include "calc.hpp"
#include <imgui.h>

class CannonRenderer;

 class Projectile 
{

public:
	
	float mass;
	float speed;
	float acceleration;
	float dragForce;
	float contactArea;
	float initialSpeed;
	float angle;
	float time = 0;

	float2 direction;
	float2 pos;
	float2 initialPos;

	Projectile() {};
	~Projectile() {};
	virtual void Update(CannonRenderer& renderer) {};

};

 class Sphere: public Projectile
 {
 public:
	 float radius;
	 Sphere() {};
	 Sphere(float radius);
	 void Update(CannonRenderer& renderer) override;
	 ~Sphere() {};
 };

