#pragma once

#include "Core/offsets.hpp"
#include "Memory/Memory.h"
#include "../Math/Vector3D.hpp"
#include "../Math/Vector2D.hpp"
#include "../Math/viewMatrix.hpp"
#include <memory>  // For std::shared_ptr

struct Camera {
    Vector2D ScreenSize;                    // Size of the screen in pixels
    std::shared_ptr<ViewMatrix> GameViewMatrix;  // Shared pointer to the view matrix
    std::mutex viewMatrixMutex;
    // Initialize the camera with screen dimensions
    void Initialize(int Width, int Height) {
        ScreenSize = Vector2D(Width, Height);
        GameViewMatrix = std::make_shared<ViewMatrix>();
    }

    // Get the current screen resolution
    const Vector2D& GetResolution() const {
        return ScreenSize;
    }

    // Update the game view matrix based on client's memory location
    void Update(uintptr_t client) {
        {
            std::lock_guard<std::mutex> lock(viewMatrixMutex); // Lock the mutex for playerList access
            *GameViewMatrix = mem.Read<ViewMatrix>(client + client_dll::dwViewMatrix); // Capture the current playerList
        }
    }

    // Convert world coordinates to screen coordinates
    // Returns true if successful, false if the point is behind the camera
    bool WorldToScreen(Vector3D WorldPosition, Vector2D& ScreenPosition) const {
        // Transform the world position using the game's view matrix

        Vector3D transformed = GameViewMatrix->Transform(WorldPosition);

        // Check if the transformed point is behind the camera
        constexpr float epsilon = 0.01f;  // Small epsilon value for depth check
        if (transformed.z < epsilon) {
            return false;  // Point is behind the camera, cannot be rendered on screen
        }

        // Perform perspective division and map to screen coordinates
        transformed.x *= 1.0f / transformed.z;
        transformed.y *= 1.0f / transformed.z;

        // Map to screen coordinates based on screen size and center
        ScreenPosition = Vector2D(
            ScreenSize.x / 2.0f + transformed.x * (ScreenSize.x / 2.0f),
            ScreenSize.y / 2.0f - transformed.y * (ScreenSize.y / 2.0f)
        );

        return true;  // Successfully mapped to screen coordinates
    }
};
