#pragma once

#include <Math/viewMatrix.hpp>
#include <Math/Vector3D.hpp>
#include <Math/Vector2D.hpp>

#include <Gui/Renderer.hpp>
#include <Utils/features.hpp>

#include <Utils/hitboxType.hpp>

#include <Core/CoreUpdater.hpp>

#include <Memory/Memory.h>

struct ESP {
    ViewMatrix vm = {};
    int selected_subtab = 1;

    void RenderUI() {
        if (ImGui::BeginTabItem("Esp", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            ImGui::BeginChild("Enemy", ImVec2(100, 0), true);
            ImGui::BeginGroup();

            float button_width = ImGui::GetContentRegionAvail().x;
            if (ImGui::Button("Enemy", ImVec2(button_width, 0))) {
                this->selected_subtab = 1;
            }
            if (ImGui::Button("Team", ImVec2(button_width, 0))) {
                this->selected_subtab = 2;
            }

            ImGui::EndGroup();
            ImGui::EndChild();
            ImGui::SameLine();

            ImGui::BeginChild("ContentRegion", ImVec2(0, 0), false);
            if (this->selected_subtab == 1) {
                ImGui::Checkbox("Enable", &Features::Esp::Enemy::DrawEnemy);
                if (Features::Esp::Enemy::DrawEnemy) {
                    ImGui::SameLine();
                    ImGui::Checkbox("DrawBoxes", &Features::Esp::Enemy::DrawBoxes);
                    if (Features::Esp::Enemy::DrawBoxes) {
                        const char* BoxTypeIndex[] = { "2D", "2D Filled", "2D Corners" };
                        ImGui::SameLine();
                        ImGui::Checkbox("Box Outline", &Features::Esp::Enemy::BoxOutline);

                        if (ImGui::BeginCombo("Combo Box", BoxTypeIndex[Features::Esp::Enemy::BoxType])) {
                            for (int n = 0; n < IM_ARRAYSIZE(BoxTypeIndex); n++) {
                                bool is_selected = (Features::Esp::Enemy::BoxType == n);
                                if (ImGui::Selectable(BoxTypeIndex[n], is_selected))
                                    Features::Esp::Enemy::BoxType = n;
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }

                        ImGui::SliderFloat("Box Thickness", &Features::Esp::Enemy::BoxThickness, 1, 10);
                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                            ImGui::SetTooltip("Changes the thickness of the boxes");

                        ImGui::ColorEdit4("Invisible Box Color", (float*)&Features::Color::Esp::Enemy::InvisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Visible Box Color", (float*)&Features::Color::Esp::Enemy::VisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                    }
                }
            }
            else if (this->selected_subtab == 2) {
                ImGui::Checkbox("Enable", &Features::Esp::Teammate::DrawTeam);
                if (Features::Esp::Teammate::DrawTeam) {
                    ImGui::SameLine();
                    ImGui::Checkbox("DrawBoxes", &Features::Esp::Teammate::DrawBoxes);
                    if (Features::Esp::Teammate::DrawBoxes) {
                        const char* BoxTypeIndex[] = { "2D", "2D Filled", "2D Corners" };

                        ImGui::SameLine();
                        ImGui::Checkbox("Box Outline", &Features::Esp::Teammate::BoxOutline);

                        if (ImGui::BeginCombo("Combo Box", BoxTypeIndex[Features::Esp::Teammate::BoxType])) {
                            for (int n = 0; n < IM_ARRAYSIZE(BoxTypeIndex); n++) {
                                bool is_selected = (Features::Esp::Teammate::BoxType == n);
                                if (ImGui::Selectable(BoxTypeIndex[n], is_selected))
                                    Features::Esp::Teammate::BoxType = n;
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }

                        ImGui::SliderFloat("Box Thickness", &Features::Esp::Teammate::BoxThickness, 1, 10);
                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                            ImGui::SetTooltip("Changes the thickness of the boxes");

                        ImGui::ColorEdit4("Invisible Box Color", (float*)&Features::Color::Esp::Teammate::InvisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                        ImGui::ColorEdit4("Visible Box Color", (float*)&Features::Color::Esp::Teammate::VisibleBoxColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
                    }
                }
            }

            ImGui::EndChild();
            ImGui::Separator();
            ImGui::EndTabItem();
        }
    }
    

    void RenderOverView(ImDrawList* CanvasOverview, ImVec2 window_pos) {
        const int Boxtype = selected_subtab == 1 ? Features::Esp::Enemy::BoxType : Features::Esp::Teammate::BoxType;
        const int BoxStyle = selected_subtab == 1 ? Features::Esp::Enemy::BoxStyle : Features::Esp::Teammate::BoxStyle;
        const bool BoxOutline = selected_subtab == 1 ? Features::Esp::Enemy::BoxOutline : Features::Esp::Teammate::BoxOutline;
        const float BoxThickness = selected_subtab == 1 ? Features::Esp::Enemy::BoxThickness : Features::Esp::Teammate::BoxThickness;

        ImVec4 BoxColor, FilledBoxColor;
        const float* ChosenColor = selected_subtab == 1 ? Features::Color::Esp::Enemy::InvisibleBoxColor : Features::Color::Esp::Teammate::InvisibleBoxColor;
        BoxColor = ImVec4(ChosenColor[0], ChosenColor[1], ChosenColor[2], ChosenColor[3]);

        const float* FilledChosenColor = selected_subtab == 1 ? Features::Color::Esp::Enemy::InvisibleFilledBoxColor : Features::Color::Esp::Teammate::InvisibleFilledBoxColor;
        FilledBoxColor = ImVec4(FilledChosenColor[0], FilledChosenColor[1], FilledChosenColor[2], FilledChosenColor[3]);

        float button_width = ImGui::GetContentRegionAvail().x;
        Vector2D head(window_pos.x + button_width / 2, window_pos.y + 80);
        Vector2D feet(window_pos.x + button_width / 2, window_pos.y + 170);

        Renderer::Draw2DBox(CanvasOverview, Boxtype, BoxStyle, BoxOutline, feet, head, BoxColor, FilledBoxColor, BoxThickness);
    }

    void RenderDrawings(ImDrawList* Canvas) {
        std::vector<Player> localPlayerList;
        BYTE localTeam;

        {
            std::lock_guard<std::mutex> lock(Core.playerListMutex);
            localPlayerList = Core.playerList;
        }

    

        uintptr_t localPlayerPawn = mem.Read< uintptr_t>(Core.client + client_dll::dwLocalPlayerPawn);
        localTeam = mem.Read<BYTE>(localPlayerPawn + C_BaseEntity::m_iTeamNum);
        ViewMatrix GameViewMatrix = mem.Read<ViewMatrix>(Core.client + client_dll::dwViewMatrix);
        Vector3D position = mem.Read<Vector3D>(localPlayerPawn + C_BasePlayerPawn::m_vOldOrigin);
   
     
        for (auto entity : localPlayerList) {

            bool DrawBoxes = localTeam == entity.team ? Features::Esp::Teammate::DrawBoxes : Features::Esp::Enemy::DrawBoxes;
            if (!DrawBoxes && localTeam == entity.team)
                continue;

            if (!entity.isAlive()) continue;

            Vector2D head, feet;
            ImVec4 BoxColor = GetBoxColor(entity, localTeam);
            ImVec4 FilledBoxColor = GetBoxColor(entity, localTeam);

            if (!Core.GameCamera.WorldToScreen(entity.boneArray[1], head)) continue;
            if (!Core.GameCamera.WorldToScreen(entity.position, feet)) continue;
            float DistanceToLocalPlayer = position.Distance(entity.position);

            if (DrawBoxes) {
                int Boxtype = GetBoxType(localTeam, entity.team);
                int BoxStyle = GetBoxStyle(localTeam, entity.team);
                bool BoxOutline = GetBoxOutline(localTeam, entity.team);
                float BoxThickness = GetBoxThickness(localTeam, entity.team);

                Renderer::Draw2DBox(Canvas, Boxtype, BoxStyle, BoxOutline, feet, head, BoxColor, FilledBoxColor, BoxThickness);
                Renderer::Draw2DBarTest(Canvas, feet, head, entity.health, 1, 1, 4, 4);
            }

            bool DrawScelet = true;
            if (DrawScelet) {
                Vector2D Waist, Head, Neck, Leftshoulder, Leftelbow, Lefthand, Rightshoulder, RightelbowBone, Righthand, Leftknees, Leftleg, Rightknees, Rightleg;
                ImColor SkeletonColor = ImColor(255, 255, 255, 255);
                int Thickness = 1;

                Core.GameCamera.WorldToScreen(entity.boneArray[0], Waist);
                Core.GameCamera.WorldToScreen(entity.boneArray[1], Head);
                Core.GameCamera.WorldToScreen(entity.boneArray[2], Neck);
                Core.GameCamera.WorldToScreen(entity.boneArray[3], Leftshoulder);
                Core.GameCamera.WorldToScreen(entity.boneArray[4], Leftelbow);
                Core.GameCamera.WorldToScreen(entity.boneArray[5], Lefthand);
                Core.GameCamera.WorldToScreen(entity.boneArray[6], Rightshoulder);
                Core.GameCamera.WorldToScreen(entity.boneArray[7], RightelbowBone);
                Core.GameCamera.WorldToScreen(entity.boneArray[8], Righthand);
                Core.GameCamera.WorldToScreen(entity.boneArray[9], Leftknees);
                Core.GameCamera.WorldToScreen(entity.boneArray[10], Leftleg);
                Core.GameCamera.WorldToScreen(entity.boneArray[11], Rightknees);
                Core.GameCamera.WorldToScreen(entity.boneArray[12], Rightleg);

                Renderer::DrawLine(Canvas, Head, Neck, Thickness, ImColor(SkeletonColor));

                Renderer::DrawLine(Canvas, Neck, Leftshoulder, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, Leftshoulder, Leftelbow, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, Leftelbow, Lefthand, Thickness, ImColor(SkeletonColor));

                Renderer::DrawLine(Canvas, Neck, Rightshoulder, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, Rightshoulder, RightelbowBone, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, RightelbowBone, Righthand, Thickness, ImColor(SkeletonColor));

                Renderer::DrawLine(Canvas, Neck, Waist, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, Waist, Leftknees, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, Leftknees, Leftleg, Thickness, ImColor(SkeletonColor));

                Renderer::DrawLine(Canvas, Waist, Rightknees, Thickness, ImColor(SkeletonColor));
                Renderer::DrawLine(Canvas, Rightknees, Rightleg, Thickness, ImColor(SkeletonColor));

                Renderer::DrawCircle(Canvas, Head, 3200 / DistanceToLocalPlayer, 255, SkeletonColor, Thickness);
            }
        }
    }

private:
    ImVec4 GetBoxColor(const Player entity, BYTE localTeam) const {
        const float* Teammate_Color = !entity.spotted ? Features::Color::Esp::Teammate::InvisibleBoxColor : Features::Color::Esp::Teammate::VisibleBoxColor;
        const float* Enemy_Color = !entity.spotted ? Features::Color::Esp::Enemy::InvisibleBoxColor : Features::Color::Esp::Enemy::VisibleBoxColor;
        const float* ChosenColor = localTeam == entity.team ? Teammate_Color : Enemy_Color;
        return ImVec4(ChosenColor[0], ChosenColor[1], ChosenColor[2], ChosenColor[3]);
    }

    int GetBoxType(BYTE localTeam, BYTE entityTeam) const {
        return localTeam == entityTeam ? Features::Esp::Teammate::BoxType : Features::Esp::Enemy::BoxType;
    }

    int GetBoxStyle(BYTE localTeam, BYTE entityTeam) const {
        return localTeam == entityTeam ? Features::Esp::Teammate::BoxStyle : Features::Esp::Enemy::BoxStyle;
    }

    bool GetBoxOutline(BYTE localTeam, BYTE entityTeam) const {
        return localTeam == entityTeam ? Features::Esp::Teammate::BoxOutline : Features::Esp::Enemy::BoxOutline;
    }

    float GetBoxThickness(BYTE localTeam, BYTE entityTeam) const {
        return localTeam == entityTeam ? Features::Esp::Teammate::BoxThickness : Features::Esp::Enemy::BoxThickness;
    }
};

inline ESP esp;
