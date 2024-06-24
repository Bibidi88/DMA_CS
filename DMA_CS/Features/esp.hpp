#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include <Math/viewMatrix.hpp>
#include <Math/Vector3D.hpp>
#include <Math/Vector2D.hpp>

#include <Gui/Renderer.hpp>
#include <Utils/features.hpp>


#include <Core/offsets.hpp>
#include <Core/client.dll.hpp>

#include <Memory/Memory.h>


struct ESP
{
    int ScreenWidth;
    int ScreenHeight;
    std::vector<uintptr_t> entities;
    ViewMatrix vm = {};

    int selected_subtab = 1;

    void RenderUI() {
        if (ImGui::BeginTabItem("Esp", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {

            ImGui::BeginChild("Enemy", ImVec2(100, 0), true);

            ImGui::BeginGroup(); // Beginne eine Gruppe für die vertikale Ausrichtung
            float button_width = ImGui::GetContentRegionAvail().x;
            if (ImGui::Button("Enemy", ImVec2(button_width, 0)))
            {
                selected_subtab = 1;
            }

            if (ImGui::Button("Team", ImVec2(button_width, 0)))
            {
                selected_subtab = 2;
            }

            ImGui::EndGroup();
            ImGui::EndChild();
            ImGui::SameLine();

            ImGui::BeginChild("ContentRegion", ImVec2(0, 0), false);
            if (selected_subtab == 1) {
                ImGui::Checkbox("Enable", &Features::Esp::Enemy::DrawEnemy);
                if (Features::Esp::Enemy::DrawEnemy) {
                    ImGui::Checkbox("DrawBoxes", &Features::Esp::Enemy::DrawBoxes);
                    if (Features::Esp::Enemy::DrawBoxes) {
                        ImGui::SameLine();
                        ImGui::Checkbox("Box Outline", &Features::Esp::Enemy::BoxOutline);
                        const char* BoxTypeIndex[] = { "2D", "2D Filled", "2D Corners" };
                        if (ImGui::BeginCombo("Combo Box", BoxTypeIndex[Features::Esp::Enemy::BoxType])) // The second parameter is the label preview
                        {
                            for (int n = 0; n < IM_ARRAYSIZE(BoxTypeIndex); n++)
                            {
                                bool is_selected = (Features::Esp::Enemy::BoxType == n); // You can store your selection in a variable or store in ImGui itself
                                if (ImGui::Selectable(BoxTypeIndex[n], is_selected))
                                    Features::Esp::Enemy::BoxType = n;
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            }
                            ImGui::EndCombo();
                        }

                        ImGui::SliderFloat("Box Thickness", &Features::Esp::Enemy::BoxThickness, 1, 10);
                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                            ImGui::SetTooltip("Changes the thickness of the boxes");

                        ImGui::ColorEdit4("Enemy Color", (float*)&Features::Color::Esp::Enemy::InvisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Enemy Visual Color", (float*)&Features::Color::Esp::Enemy::VisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Enemy FilledColor", (float*)&Features::Color::Esp::Enemy::InvisibleFilledBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Enemy Visual FilledColor", (float*)&Features::Color::Esp::Enemy::VisibleFilledBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                    }
                }
            }

            else if (selected_subtab == 2) {
                ImGui::Checkbox("Enable", &Features::Esp::Teammate::DrawTeam);
                if (Features::Esp::Teammate::DrawTeam) {
                    ImGui::Checkbox("Draw Boxes", &Features::Esp::Teammate::DrawBoxes);
                    if (Features::Esp::Teammate::DrawBoxes) {
                        ImGui::SameLine();
                        ImGui::Checkbox("TeamBox Outline", &Features::Esp::Teammate::BoxOutline);
                        const char* BoxTypeIndex[] = { "2D", "2D Filled", "2D Corners" };
                        if (ImGui::BeginCombo("Combo TeamBox", BoxTypeIndex[Features::Esp::Teammate::BoxType])) // The second parameter is the label preview
                        {
                            for (int n = 0; n < IM_ARRAYSIZE(BoxTypeIndex); n++)
                            {
                                bool is_selected = (Features::Esp::Teammate::BoxType == n); // You can store your selection in a variable or store in ImGui itself
                                if (ImGui::Selectable(BoxTypeIndex[n], is_selected))
                                    Features::Esp::Teammate::BoxType = n;
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::SliderFloat("TeamBox Thickness", &Features::Esp::Teammate::BoxThickness, 1, 10);

                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                            ImGui::SetTooltip("Changes the thickness of the boxes");

                        ImGui::ColorEdit4("Team Color", (float*)&Features::Color::Esp::Teammate::InvisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Team Visual Color", (float*)&Features::Color::Esp::Teammate::VisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Team FilledColor", (float*)&Features::Color::Esp::Teammate::InvisibleFilledBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Team Visual FilledColor", (float*)&Features::Color::Esp::Teammate::VisibleFilledBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                    }
                }
            }

            ImGui::EndChild();

            ImGui::Separator();
            ImGui::EndTabItem();
        }
        
    }

    void RenderOverView(ImDrawList* CanvasOverview, ImVec2 window_pos) {

        int Boxtype = selected_subtab == 1 ? Features::Esp::Enemy::BoxType : Features::Esp::Teammate::BoxType;
        int BoxStyle = selected_subtab == 1 ? Features::Esp::Enemy::BoxStyle : Features::Esp::Teammate::BoxStyle;
        bool BoxOutline = selected_subtab == 1 ? Features::Esp::Enemy::BoxOutline : Features::Esp::Teammate::BoxOutline;
        float BoxThickness = selected_subtab == 1 ? Features::Esp::Enemy::BoxThickness : Features::Esp::Teammate::BoxThickness;

        ImVec4 FilledBoxColor, BoxColor;

        
         float* ChosenColor = selected_subtab == 1 ? Features::Color::Esp::Enemy::InvisibleBoxColor : Features::Color::Esp::Teammate::InvisibleBoxColor;
         BoxColor = ImVec4(ChosenColor[0], ChosenColor[1], ChosenColor[2], ChosenColor[3]);

         float* FilledChosenColor = selected_subtab == 1 ? Features::Color::Esp::Enemy::InvisibleFilledBoxColor : Features::Color::Esp::Teammate::InvisibleFilledBoxColor;
         FilledBoxColor = ImVec4(FilledChosenColor[0], FilledChosenColor[1], FilledChosenColor[2], FilledChosenColor[3]);

         float button_width = ImGui::GetContentRegionAvail().x;
         Vector2D head = Vector2D(window_pos.x + button_width / 2, window_pos.y + 80);
         Vector2D feet = Vector2D(window_pos.x + button_width / 2, window_pos.y + 170);


        Renderer::Draw2DBox(
            CanvasOverview, 
            Boxtype, 
            BoxStyle, 
            BoxOutline, 
            feet,
            head,
            BoxColor, 
            FilledBoxColor,
            BoxThickness
        );
    }

    void RenderDrawings(ImDrawList* Canvas, auto client) {

        uintptr_t entity_list = mem.Read<uintptr_t>(client + client_dll::dwEntityList);

        uintptr_t localPlayerPawn = mem.Read< uintptr_t>(client + client_dll::dwLocalPlayerPawn);
        BYTE team = mem.Read<BYTE>(localPlayerPawn + C_BaseEntity::m_iTeamNum);

        std::vector<uintptr_t> buffer = {};

        for (int i = 1; i < 32; ++i) {
            uintptr_t listEntry = mem.Read<uintptr_t>(entity_list + ((8 * (i & 0x7ff) >> 9) + 16));
            if (!listEntry) continue;

            uintptr_t entityController = mem.Read<uintptr_t>(listEntry + 120 * (i & 0x1ff));
            if (!entityController) continue;

            uintptr_t entityControllerPawn = mem.Read<uintptr_t>(entityController + CCSPlayerController::m_hPlayerPawn);
            if (!entityControllerPawn) continue;

            if (entityControllerPawn == localPlayerPawn) continue;

            uintptr_t entity = mem.Read<uintptr_t>(listEntry + 120 * (entityControllerPawn & 0x1ff));
            int health = mem.Read<int>(entity + C_BaseEntity::m_iHealth);
            if (health <= 0 || health > 100) continue;

            if (entity) buffer.emplace_back(entity);
        }

        entities = buffer;

        vm = mem.Read<ViewMatrix>(client + client_dll::dwViewMatrix);
        GrabScreenSize();

        
        for (uintptr_t entity : entities)
        {
            // Team Check
            int entityTeam = mem.Read<int>(entity + C_BaseEntity::m_iTeamNum);
            if (!Features::Esp::Teammate::DrawBoxes && team == entityTeam) 
                continue;

            Vector3D absOrigin = mem.Read<Vector3D>(entity + C_BasePlayerPawn::m_vOldOrigin);
            Vector3D eyePos = absOrigin.Add(mem.Read<Vector3D>(entity + C_BaseModelEntity::m_vecViewOffset));

            Vector2D head, feet;
           
            bool DrawBoxes = team == entityTeam ? Features::Esp::Teammate::DrawBoxes : Features::Esp::Enemy::DrawBoxes;

            auto GetColor = [entity](const BYTE team, const BYTE entityTeam, const float* Teammate_VisColor, const float* Teammate_InvisColor, const float* Enemy_VisColor, const float* Enemy_InvisColor) {
               // const float* Teammate_Color =  p->IsVisible ? Teammate_VisColor : Teammate_InvisColor;
               // const float* Enemy_Color = p->IsVisible ? Enemy_VisColor : Enemy_InvisColor;
                const float* ChosenColor = team == entityTeam ? Teammate_InvisColor : Enemy_InvisColor;
                return ImVec4(ChosenColor[0], ChosenColor[1], ChosenColor[2], ChosenColor[3]);
            };

            ImVec4 BoxColor = GetColor(team, entityTeam, Features::Color::Esp::Teammate::VisibleBoxColor, Features::Color::Esp::Teammate::InvisibleBoxColor, Features::Color::Esp::Enemy::VisibleBoxColor, Features::Color::Esp::Enemy::InvisibleBoxColor);
            ImVec4 FilledBoxColor = GetColor(team, entityTeam, Features::Color::Esp::Teammate::VisibleFilledBoxColor, Features::Color::Esp::Teammate::InvisibleFilledBoxColor, Features::Color::Esp::Enemy::VisibleFilledBoxColor, Features::Color::Esp::Enemy::InvisibleFilledBoxColor);


            if (!WorldToScreen(absOrigin, head)) continue;
            if (!WorldToScreen(eyePos, feet)) continue;
               
            if (DrawBoxes) {
                int Boxtype = team == entityTeam ? Features::Esp::Teammate::BoxType : Features::Esp::Enemy::BoxType;
                int BoxStyle = team == entityTeam ? Features::Esp::Teammate::BoxStyle : Features::Esp::Enemy::BoxStyle;
                bool BoxOutline = team == entityTeam ? Features::Esp::Teammate::BoxOutline : Features::Esp::Enemy::BoxOutline;
                float BoxThickness = team == entityTeam ? Features::Esp::Teammate::BoxThickness : Features::Esp::Enemy::BoxThickness;

                Renderer::Draw2DBox(Canvas, Boxtype, BoxStyle, BoxOutline, feet, head, BoxColor, FilledBoxColor, BoxThickness);
            }
                
        }

    }

    bool WorldToScreen(Vector3D WorldPosition, Vector2D& ScreenPosition) const {
        Vector3D transformed = vm.Transform(WorldPosition);

        if (transformed.z < 0.001) {
            return false;
        }

        transformed.x *= 1.0 / transformed.z;
        transformed.y *= 1.0 / transformed.z;

        ScreenPosition = Vector2D(ScreenWidth / 2.0f + transformed.x * (ScreenWidth / 2.0f), ScreenHeight / 2.0f - transformed.y * (ScreenHeight / 2.0f));

        return true;
    }

    void GrabScreenSize() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);
        ScreenWidth = vidMode->width;
        ScreenHeight = vidMode->height;
    }
};

