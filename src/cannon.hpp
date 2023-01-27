#pragma once

#include <imgui.h>
#include <vector>
#include "types.hpp"
#include "Projectile.hpp"
#include "Time.h"

void movePoint(float2& coordinate, float dir, float unit);

struct Line
{
    Line() { start = { 50,100 }; end = { -150,-100 }; };
    float2 start;
    float2 end;
};

class colorGrad {
public:
    colorGrad() { current = 255; next = 255; };
    void Update(float step);
    float current;
    float next;
    float colorTemp;
    float lerpColor;
};

struct Target
{
    float2 position;
    int score;
    float width;
    float height;
    float dir;
};

struct CannonState
{
    float2 position;
    float angle = 45; // In radians
    float width;
    float height;
    float initialSpeed = 25;
    float decelerationDuCanon;
};

// Renderer class
// Used to load resources
class CannonRenderer
{
public:
    CannonRenderer();
    ~CannonRenderer();

    void PreUpdate();

    // Fast access to drawlist and io
    ImDrawList* dl;
    ImGuiIO* io;

    // World coordinates conversion (world is expressed in meters)
    float2 worldOrigin; // Origin in pixels
    float2 worldScale;  // Scale relative to pixels
    float2 ToPixels(float2 coordinatesInMeters);
    float2 ToWorld(float2 coordinatesInPixels);

    // Draw functions
    void DrawGround();
    void ToShoot();
    void DrawCannon(const CannonState& cannon);
    void DrawProjectileMotion(const CannonState& cannon);
};

class CannonGame
{
public:
    CannonGame(CannonRenderer& renderer);
    ~CannonGame();

    void UpdateAndDraw();

private:
    colorGrad upprLeft;
    colorGrad upprRight;
    colorGrad bottmLeft;
    colorGrad bottmRight;
    std::vector<Line> stylisedLines;

    float LineDelay;
    float LineOpacity;
    int opacity;

    Time time;
    CannonRenderer& renderer;
    CannonState cannonState;
    Target target;
    std::vector<Projectile*> projectiles;
    Sphere sphereDisplay;

};
