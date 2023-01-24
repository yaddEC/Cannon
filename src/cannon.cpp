#include <stdio.h>
#include <math.h>

#include "calc.hpp"
#include "cannon.hpp"
#include "Maths.hpp"

CannonRenderer::CannonRenderer()
{

    // Do not use ImDrawList or ImGuiIO here because they are invalid at this point
    // Use this function only to load resources (images, sounds) if needed
}

CannonRenderer::~CannonRenderer()
{
    // Unload resources if needed
}

void CannonRenderer::PreUpdate()
{
    // Get frequently used variables
    dl = ImGui::GetBackgroundDrawList();
    io = &ImGui::GetIO();

    // Compute world space
    // ===================
    // Origin:
    //  - x at the center of the window
    //  - y at the 3/4 bottom of the window
    // Scale:
    //  50 meters from left of the window to the right
    //  Uniform scale in y except it's pointing to up
    worldOrigin.x = io->DisplaySize.x / 2.f;
    worldOrigin.y = io->DisplaySize.y - io->DisplaySize.y / 4.f;
    worldScale.x  = io->DisplaySize.x / 50.f;
    worldScale.y  = -worldScale.x; // Same scale as X bu invert it to invert y coordinates
}

float2 CannonRenderer::ToPixels(float2 coordinatesInMeters)
{
    return (coordinatesInMeters * worldScale) + worldOrigin;
}

float2 CannonRenderer::ToWorld(float2 coordinatesInPixels)
{
    return (coordinatesInPixels - worldOrigin) / worldScale;
}

void CannonRenderer::DrawGround()
{
    float2 left  = this->ToPixels({ -100.f, 0.f });
    float2 right = this->ToPixels({ +100.f, 0.f });

    dl->AddLine(left, right, IM_COL32_WHITE);
}

void CannonRenderer::DrawCannon(const CannonState& cannon)
{
    // TODO: Draw cannon
    
    // For example (Remove this and do your own)
    float2 pos = this->ToPixels(cannon.position);
    float2 cannonEdge[4];
    float2 cannonOrigin;
    cannonEdge[0].x = pos.x - cannon.width;
    cannonEdge[0].y = pos.y - cannon.height*2;

    cannonEdge[1].x = pos.x - cannon.width;
    cannonEdge[1].y = pos.y ;

    cannonEdge[2].x = pos.x + cannon.width;
    cannonEdge[2].y = pos.y - cannon.height*2;

    cannonEdge[3].x = pos.x + cannon.width;
    cannonEdge[3].y = pos.y;
    
    cannonOrigin.x = pos.x;
    cannonOrigin.y = pos.y ;



    for (int i = 0; i < 4; i++)
    {
        cannonEdge[i] = rotateVec(cannonEdge[i], cannonOrigin,90 - cannon.angle);
    }
    
    dl->AddCircle(pos, 10.f, IM_COL32_WHITE);

    dl->AddLine(cannonEdge[0], cannonEdge[1], IM_COL32_WHITE);
    dl->AddLine(cannonEdge[2], cannonEdge[3], IM_COL32_WHITE);
    dl->AddLine(cannonEdge[0], cannonEdge[2], IM_COL32_WHITE);
    dl->AddLine(cannonEdge[1], cannonEdge[3], IM_COL32_WHITE);
}

void CannonRenderer::DrawProjectileMotion(const CannonState& cannon)
{
    // TODO: Draw cannon projectile using ImDrawList
    // e.g. dl->AddLine(...)
    // e.g. dl->PathLineTo(...)
    // etc...
}

CannonGame::CannonGame(CannonRenderer& renderer)
    : renderer(renderer)
{
    time.Init();

    cannonState.position.x = -22.f;
    cannonState.height = 50;
    cannonState.width = 25;
    Sphere displaySphere;
    // TODO: Init cannonState here
}

CannonGame::~CannonGame()
{

}

void CannonGame::UpdateAndDraw()
{
    renderer.PreUpdate();
    time.Update();


    if (ImGui::Begin("Canon state", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // TODO: Add UI to edit other cannon state variables here
        ImGui::SliderFloat("Cannon Position Y", &cannonState.position.y, 0, 15.f);
        ImGui::SliderFloat("Cannon Position X", &cannonState.position.x, -25.f, 15.f);
        ImGui::SliderFloat("Angle", &cannonState.angle, 0.f, 90.f);
        ImGui::SliderFloat("Width", &cannonState.width, 0.f, 200.f);
        ImGui::SliderFloat("Height", &cannonState.height, 0.f, 200.f);
        ImGui::SliderFloat("Initial Speed", &cannonState.initialSpeed, 0.f, 200.f);
    }
    ImGui::End();

    if (ImGui::Begin("Projectile", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // TODO: Add UI to edit other cannon state variables here
        const char* items[] = { "Sphere", "Cube"};
        static int item_current = 0;
        ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
       
     

        if (item_current == 0)
        {
            ImGui::SliderFloat("radius", &sphereDisplay.radius, 0.1, cannonState.width);
            ImGui::SliderFloat("mass", &sphereDisplay.mass, 0.1, 15.f);
            ImGui::SliderFloat("initial speed", &sphereDisplay.initialSpeed, 0.1, 100.f);
            if (ImGui::Button("SHOOT!!!!!", ImVec2(100,50)))
            {
                
                if (sphereDisplay.radius > cannonState.width)
                    projectiles.push_back(new Sphere(cannonState.width, sphereDisplay.initialSpeed, cannonState.angle, cannonState.position));
                else
                    projectiles.push_back(new Sphere(sphereDisplay.radius, sphereDisplay.initialSpeed, cannonState.angle, cannonState.position));

                projectiles.back()->mass = sphereDisplay.mass;
                projectiles.back()->Init();
            }
        }


    }
    ImGui::End();

    // Draw cannon
    renderer.DrawGround();
    renderer.DrawCannon(cannonState);
    renderer.DrawProjectileMotion(cannonState);
    for (auto it = projectiles.begin(); it != projectiles.end(); )
    {

        (*it)->Update(renderer);
        if ((*it)->position.x > 25 || (*it)->position.x < -25)
        {
            it = projectiles.erase(it);
        }
        else
        {
            it++;
        }
    }
}
