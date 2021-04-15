#include "debug_gui.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/brawler_character.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/systems/physics.h"

using namespace dagger;
using namespace brawler;

void DebugGui::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&DebugGui::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().connect<&DebugGui::RenderToolMenu>(this);
}

void DebugGui::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&DebugGui::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&DebugGui::RenderToolMenu>(this);
}

void DebugGui::RenderToolMenu()
{
    if (ImGui::BeginMenu("Brawler"))
    {
        if (ImGui::MenuItem("Reset Position"))
        {
            resetPosition = true;
        }

        ImGui::EndMenu();
    }

    ImGui::Begin("Brawler Debug", &debugActive);

    Engine::Registry().view<Transform, Movable, Player>().each([&](Transform& t, Movable& m, Player& p)
        {
            ImGui::Text("Position: %f, %f", t.position.x, t.position.y);
            ImGui::Text("Speed: %f, %f", m.speed.x, m.speed.y);
            ImGui::Separator();
        });

    ImGui::Text("Physics");
    ImGui::SliderFloat("Gravity", &PhysicsSystem::s_Gravity, 0.0f, 1000.0f);
    ImGui::SliderFloat("Drag Speed", &PhysicsSystem::s_DragSpeed, 0.0f, 200.0f);
    ImGui::Separator();

    ImGui::Text("Movement");
    ImGui::SliderFloat("Run Speed", &PhysicsSystem::s_RunSpeed, 0.0f, 400.0f);
    ImGui::SliderFloat("Jump Speed", &PhysicsSystem::s_JumpSpeed, 0.0f, 800.0f);
    ImGui::SliderFloat("Terminal Velocity", &PhysicsSystem::s_TerminalVelocity, 0.0f, 1000.0f);
    ImGui::SliderFloat("Air Mobility", &PhysicsSystem::s_AirMobility, 0.0f, 1.0f);

    ImGui::End();
}

void DebugGui::Run()
{
}

void DebugGui::OnEndOfFrame()
{
    if(resetPosition) {
        //TODO reposition player
        Engine::Registry().view<Transform, Movable, Player>().each([&](Transform& t, Movable& m, Player& p)
            {
                t.position.x = 0.0f;
                t.position.y = 0.0f;
                m.speed.x = 0.0f;
                m.speed.y = 0.0f;
            });
        resetPosition = false;
    }
}