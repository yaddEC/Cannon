#pragma once

#include <imgui.h>
#include <vector>
#include "types.hpp"
#include "Projectile.hpp"
#include "Time.h"


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
    float CannonExit(float a, float L, float IS);
};

class CannonGame
{
public:
    CannonGame(CannonRenderer& renderer);
    ~CannonGame();

    void UpdateAndDraw();

private:
    Time time;
    CannonRenderer& renderer;
    CannonState cannonState;
    std::vector<Projectile*> projectiles;
    Sphere sphereDisplay;

};
