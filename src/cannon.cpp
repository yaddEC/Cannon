#include <stdio.h>
#include <math.h>
#include <random>
#include <string>

#include "calc.hpp"
#include "cannon.hpp"
#include "Maths.hpp"




void movePoint(float2& coordinate, float dir, float unit)
{
    coordinate.x += unit * cos(dir);
    coordinate.y += unit * sin(dir);
}

void generateRandomColor(float& next)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    next = dis(gen);
}


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

bool CannonRenderer::circleCircleCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    return distance < (r1 + r2);
    
}

void CannonRenderer::DrawGround()
{
    float2 left = this->ToPixels({ -100.f, 0.f });
    float2 right = this->ToPixels({ +100.f, 0.f });

    dl->AddLine(left, right, IM_COL32_WHITE, 4);
}

void CannonRenderer::DrawBasket()
{
    float2 Bar1 = this->ToPixels({20.f, 13.f});
    float2 Bar2 = this->ToPixels({ 20.f, 0.f });
    float2 Bar3 = this->ToPixels({ 21.f, 13.f });
    float2 Bar4 = this->ToPixels({ 21.f, 0.f });
    float2 Basket1 = this->ToPixels({ 15.f, 10.f });
    float2 Basket2 = this->ToPixels({ 20.f, 10.f });
    float2 Basket3 = this->ToPixels({ 16.f, 7.f });
    float2 Basket4 = this->ToPixels({ 19.f, 7.f });

    dl->AddLine(Basket1, Basket2, IM_COL32_WHITE, 4);
    dl->AddLine(Basket3, Basket4, IM_COL32_WHITE, 4);
    dl->AddLine({ Basket2.x - 10, Basket2.y}, Basket4, IM_COL32_WHITE, 4);
    dl->AddLine(Basket3, { Basket1.x + 10, Basket1.y }, IM_COL32_WHITE, 4);
    dl->AddLine(Bar1, Bar2, IM_COL32_WHITE, 4);
    dl->AddLine(Bar3, Bar4, IM_COL32_WHITE, 4);
    dl->AddLine(Bar1, Bar3, IM_COL32_WHITE, 4);
}

void CannonRenderer::DrawCannon(const CannonState& cannon)
{
    // TODO: Draw cannon

    // For example (Remove this and do your own)
    float2 pos = cannon.position;
    float2 cannonEdge[4];
    float2 cannonOrigin;
    cannonEdge[0].x = pos.x - cannon.width;
    cannonEdge[0].y = pos.y - cannon.height * 2;

    cannonEdge[1].x = pos.x - cannon.width;
    cannonEdge[1].y = pos.y;

    cannonEdge[2].x = pos.x + cannon.width;
    cannonEdge[2].y = pos.y - cannon.height * 2;

    cannonEdge[3].x = pos.x + cannon.width;
    cannonEdge[3].y = pos.y;

    cannonOrigin = ToPixels(pos);



    for (int i = 0; i < 4; i++)
    {
        cannonEdge[i] = ToPixels(cannonEdge[i]);
        cannonEdge[i] = rotateVec(cannonEdge[i], cannonOrigin, 270 - cannon.angle);
    }

    dl->AddCircle(cannonOrigin, 10.f, IM_COL32_WHITE, 0, 4);

    dl->AddLine(cannonEdge[0], cannonEdge[1], IM_COL32_WHITE, 4);
    dl->AddLine(cannonEdge[2], cannonEdge[3], IM_COL32_WHITE, 4);
    dl->AddLine(cannonEdge[0], cannonEdge[2], IM_COL32_WHITE, 4);
    dl->AddLine(cannonEdge[1], cannonEdge[3], IM_COL32_WHITE, 4);
    dl->AddCircleFilled(cannonEdge[0], 5, IM_COL32_WHITE);
    dl->AddCircleFilled(cannonEdge[1], 5, IM_COL32_WHITE);
    dl->AddCircleFilled(cannonEdge[2], 5, IM_COL32_WHITE);
    dl->AddCircleFilled(cannonEdge[3], 5, IM_COL32_WHITE);
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
    cannonState.height = 2;
    cannonState.width = 1;
    target.position.x = 0;
    target.position.y = 0;
    target.height = 10;
    target.width = 20;
    target.score = 0;
    Sphere displaySphere;
    LineDelay = 0;
    opacity = 1;
    LineOpacity = 0;
    // TODO: Init cannonState here
}

CannonGame::~CannonGame()
{

}



void CannonGame::UpdateAndDraw()
{
    LineOpacity += Time::GetDeltaTime() * 20 * opacity;
    if (LineOpacity > 40 || LineOpacity < 0)
    {
        opacity *= -1;
        if (LineOpacity < 0)
        {
            LineOpacity = 0;
        }
        if (LineOpacity > 40)
        {
            LineOpacity = 40;
        }
    }

    LineDelay += Time::GetDeltaTime();
    if (LineDelay > 1)
    {
        LineDelay = 0;
        if (stylisedLines.size() < 13)
        {
            stylisedLines.push_back(Line());
        }
    }
    upprLeft.Update(0.5);
    upprRight.Update(0.5);

    bottmLeft.Update(0.25);
    bottmRight.Update(0.25);

    cannonState.Update();
    renderer.PreUpdate();
    time.Update();


    const char* items[] = { "Sphere", "Cube" };
    static int item_current = 0;


    renderer.dl->AddRectFilledMultiColor({ 0,0 }, { renderer.io->DisplaySize.x,renderer.io->DisplaySize.y }, IM_COL32(0, 0, upprLeft.colorTemp / 1.5, 255), IM_COL32(0, 0, upprRight.colorTemp / 1.5, 255), IM_COL32(0, 0, bottmLeft.colorTemp / 2, 255), IM_COL32(0, 0, bottmRight.colorTemp / 2, 255));
    for (auto it = stylisedLines.begin(); it != stylisedLines.end(); )
    {
        movePoint((*it).start, 30, 0.1f);
        movePoint((*it).end, 30, 0.1f);

        renderer.dl->AddLine(renderer.ToPixels((*it).start), renderer.ToPixels((*it).end), IM_COL32(255, 255, 255, LineOpacity), 0.1f);
        if ((*it).start.x > 61.5)
        {
            it = stylisedLines.erase(it);
        }
        else
        {
            it++;
        }
    }

    std::string score = {"Score = " + std::to_string(scoreBasket)};

    renderer.dl->AddText(renderer.ToPixels({ 20, 20 }), IM_COL32_WHITE, score.c_str());


    if (ImGui::Begin("Canon state", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        
        // TODO: Add UI to edit other cannon state variables here
        if (ImGui::CollapsingHeader("Position", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent(10);
            ImGui::SliderFloat("Cannon Position Y", &cannonState.position.y, 0, 15.f);
            ImGui::SliderFloat("Cannon Position X", &cannonState.position.x, -25.f, 15.f);
            ImGui::Unindent(10);
        }
        if (ImGui::CollapsingHeader("Cannon Measurement", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent(10);
            ImGui::SliderFloat("Angle", &cannonState.angle, 0.f, 90.f);
            ImGui::SliderFloat("Width", &cannonState.width, 0.f, 2.f);
            ImGui::SliderFloat("Height", &cannonState.height, 0.f, 4.f);
            ImGui::SliderFloat("Mass", &cannonState.mass, 1.f, 40.f);
            ImGui::SliderFloat("Ground Friction Coefficient", &cannonState.groundFrictionCoeff, 0.f, 10.f);
            ImGui::Unindent(10);
        }
        if (ImGui::CollapsingHeader("Cannon Projectile Data", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent(10);
            ImGui::SliderFloat("Initial Speed", &cannonState.initialSpeed, 0.f, 200.f);
            ImGui::SliderFloat("Deceleration du au Canon", &cannonState.decelerationDuCanon, -200.f, 0.f);

            ImGui::SliderFloat("mass", &sphereDisplay.mass, 1, 15.f);
            const char* frictions[] = { "Linear","Quadratic","None" };
            static int friction_current = 0;
            ImGui::Combo("Friction", &friction_current, frictions, IM_ARRAYSIZE(frictions));
            ImGui::Unindent(10);

            ImGui::Indent(150);
            if (ImGui::Button("SHOOT!!!!!", ImVec2(100, 50)))
            {
                projectiles.push_back(new Sphere(cannonState.width, cannonState.initialSpeed, cannonState.angle, cannonState.position));

                projectiles.back()->canonHeight = cannonState.height;
                projectiles.back()->canonWidth = cannonState.width;
                projectiles.back()->frictionState = static_cast<ProjectileFriction>(friction_current);
                projectiles.back()->canonAngle = cannonState.angle;
                projectiles.back()->canonDeceleration = cannonState.decelerationDuCanon;
                projectiles.back()->canonInitalSpeed = cannonState.initialSpeed;
                projectiles.back()->mass = sphereDisplay.mass;
                projectiles.back()->Init();

                cannonState.cannonSpeed -= projectiles.back()->mass * cannonState.initialSpeed * cos(degToRad(cannonState.angle)) / cannonState.mass;
                printf("cannon speed = %f \n", cannonState.cannonSpeed);
            }
            ImGui::Unindent(150);
        }
    }
    ImGui::End();

    // Draw cannon
    renderer.DrawGround();
    renderer.DrawBasket();
    renderer.DrawCannon(cannonState);
    renderer.DrawProjectileMotion(cannonState);
    for (auto it = projectiles.begin(); it != projectiles.end(); )
    {
        (*it)->Update(renderer, scoreBasket);
        if ((*it)->timeAlive > 2)
        {
            it = projectiles.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void colorGrad::Update(float step)
{
    if (lerpColor >= 1)
    {
        current = colorTemp;
        generateRandomColor(next);
        lerpColor = 0;
    }
    else
    {
        lerpColor += step * Time::GetDeltaTime();
        colorTemp = lerp(current, next, lerpColor);
    }
}

void CannonState::Update()
{
    if (cannonSpeed < 0 && position.x >= -25.f)
    {
        cannonSpeed += groundFrictionCoeff * mass * GRAVITY * Time::GetDeltaTime();

        if (cannonSpeed > -0.001f)
            cannonSpeed = 0;

        position.x += cannonSpeed * Time::GetDeltaTime();
    }

    if (position.x <= -25.f)
    {
        position.x = -25.f;
        cannonSpeed = 0.f;
    }
}