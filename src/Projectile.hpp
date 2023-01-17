#pragma once
#include "calc.hpp"
#include <imgui.h>


 class Projectile 
{

public:
	
	float mass;
	float speed;
	float acceleration;
	float dragForce;
	float contactArea;
	float2 direction;
	float initialSpeed;
	float2 pos;
	float angle;
	Projectile() {};
	~Projectile() {};
	virtual void Update(ImDrawList* dl) {};

};

 class Sphere: public Projectile
 {
 public:
	 float radius;
	 Sphere() {};
	 Sphere(float radius);
	 void Update(ImDrawList* dl) override;
	 ~Sphere() {};
 };

