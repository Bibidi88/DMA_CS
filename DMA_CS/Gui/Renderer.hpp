#pragma once

#include <imgui.h>


namespace Renderer {

    static void DrawLine(ImDrawList* canvas, const Vector2D& start, const Vector2D& end, float thickness, const ImColor& color) {
        canvas->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color), thickness);
    }

    static void DrawCorneredBox(ImDrawList* canvas, float X, float Y, float W, float H, ImColor color, float thickness) {
        float lineW = (W / 4);
        float lineH = (H / 5.5);
        float lineT = -thickness;
        // Corners
        DrawLine(canvas, Vector2D(X, Y + thickness / 2), Vector2D(X, Y + lineH), thickness, color); // bot right vert
        DrawLine(canvas, Vector2D(X + thickness / 2, Y), Vector2D(X + lineW, Y), thickness, color);
        DrawLine(canvas, Vector2D(X + W - lineW, Y), Vector2D(X + W - thickness / 2, Y), thickness, color); // bot left hor
        DrawLine(canvas, Vector2D(X + W, Y + thickness / 2), Vector2D(X + W, Y + lineH), thickness, color);
        DrawLine(canvas, Vector2D(X, Y + H - lineH), Vector2D(X, Y + H - (thickness / 2)), thickness, color); // top right vert
        DrawLine(canvas, Vector2D(X + thickness / 2, Y + H), Vector2D(X + lineW, Y + H), thickness, color);
        DrawLine(canvas, Vector2D(X + W - lineW, Y + H), Vector2D(X + W - thickness / 2, Y + H), thickness, color); // top left hor
        DrawLine(canvas, Vector2D(X + W, Y + H - lineH), Vector2D(X + W, Y + H - (thickness / 2)), thickness, color);
        /*if (Outline) {
            ImColor Black = ImColor(0, 0, 0);
            DrawLine(canvas,Vector2D(X - lineT, Y - lineT +thickness/2), Vector2D(X - lineT, Y + lineH), thickness , Black); //bot right vert
            DrawLine(canvas,Vector2D(X - lineT +thickness/2, Y - lineT), Vector2D(X + lineW, Y - lineT), thickness , Black);
            DrawLine(canvas,Vector2D(X + W - lineW, Y - lineT), Vector2D(X + W - (thickness/ 2) + lineT, Y - lineT), thickness , Black); //bot left hor
            DrawLine(canvas,Vector2D(X + W + lineT, Y - lineT +thickness/2), Vector2D(X + W + lineT, Y + lineH), thickness , Black);
            DrawLine(canvas,Vector2D(X - lineT, Y + H - lineH), Vector2D(X - lineT, Y + H -(thickness/2) + lineT ), thickness , Black); //top right vert
            DrawLine(canvas,Vector2D(X +thickness/2 - lineT, Y + H + lineT), Vector2D(X + lineW, Y + H + lineT), thickness , Black); //top right hor
            DrawLine(canvas,Vector2D(X + W - lineW, Y + H + lineT), Vector2D(X + W - (thickness/ 2) + lineT , Y + H + lineT), thickness , Black); //top left hor
            DrawLine(canvas,Vector2D(X + W + lineT, Y + H - lineH), Vector2D(X + W + lineT, Y + H + lineT - (thickness/ 2)), thickness , Black);
        }*/
    }

    // DrawBox
    static void Draw2DBox(ImDrawList* canvas, int Type, int Style, bool Outline, Vector2D& foot, const Vector2D& head, const ImColor& color2D, const ImColor& Filledcolor, float thickness) {
        // Type = 2D, 2D Filled
        // Style = 1 or 2, idk what to call them (for now)
        if (Type == 0) { // 2D Box
            if (Style == 0) {
                float height = head.y - foot.y;
                float width = height / 2.0f;
                if (Outline) {
                    canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 1);
                }
                canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), color2D, 0.0f, 0, thickness);
            }
            if (Style == 1) {
                float Height = (head.y - foot.y);
                Vector2D rectTop = Vector2D(head.x - Height / 3, head.y);
                Vector2D rectBottom = Vector2D(foot.x + Height / 3, foot.y);
                if (Outline) {
                    canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 1);
                }
                canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), color2D, 0.0f, 0, thickness);
            }
        }
        if (Type == 1) { // 2D Box + 2D Filled Box
            if (Style == 0) {
                float height = head.y - foot.y;
                float width = height / 2.0f;
                if (Outline) {
                    canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 1);
                }
                canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), color2D, 0.0f, 0, thickness);
                canvas->AddRectFilled(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), Filledcolor, 0.0f, 0);
            }
            if (Style == 1) {
                float Height = (head.y - foot.y);
                Vector2D rectTop = Vector2D(head.x - Height / 3, head.y);
                Vector2D rectBottom = Vector2D(foot.x + Height / 3, foot.y);
                if (Outline) {
                    canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 1);
                }
                canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), color2D, 0.0f, 0, thickness);
                canvas->AddRectFilled(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), Filledcolor, thickness);
            }
        }
        if (Type == 2) { // 2D Corners
            float height = head.y - foot.y;
            float width = height / 2.0f;
            float x = foot.x - (width / 2.f);

            if (Outline) {
                Renderer::DrawCorneredBox(canvas, x, foot.y, width, height + (height * 0.2), ImColor(0, 0, 0), thickness + 1);
            }
            Renderer::DrawCorneredBox(canvas, x, foot.y, width, height + (height * 0.2), color2D, thickness);
        }
    }

    

   

	void DrawRect(Vector2D pos1, Vector2D pos2, ImU32 color, int thickness) {
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, 0, 0, 1);
	}
};